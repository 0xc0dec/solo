--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

-- TODO avoid using globals in functions

dofile("../../src/demos/common/Common.lua")

dev = solo.device
loader = dev:getAssetLoader()
physics = dev:getPhysics()
renderer = dev:getRenderer()
logger = dev:getLogger()
scene = solo.Scene.create(dev)

function initMainCamera()
    local node = scene:createNode()
        
    local t = node:findComponent("Transform")
    t:setLocalPosition(vec3(10, 10, 10))
    t:lookAt(vec3(0, 0, 0), vec3(0, 1, 0))
    
    local cam = node:addComponent("Camera")
    cam:setClearColor(vec4(0.0, 0.6, 0.6, 1.0))
    cam:setNear(0.05)

    return cam
end


local layout = solo.VertexBufferLayout()
layout:addAttribute(2, 0)
layout:addAttribute(3, 1)

local meshData1 = {
	0.9, 0.9, 0.0, 0.0, 1.0,
    -0.9, 0.9, 0.0, 1.0, 0.0,
    -0.9, -0.9, 1.0, 0.0, 0.0
}

local meshData2 = {
	-0.8, -0.9, 1.0, 0.0, 0.0,
    0.9, -0.9, 0.0, 1.0, 0.0,
    0.9, 0.8, 0.0, 0.0, 1.0
}

local mesh = solo.Mesh.create(dev)
mesh:addVertexBuffer(layout, meshData1, 3)
mesh:addVertexBuffer(layout, meshData2, 3)

camera = initMainCamera()
local effect = solo.Effect.createFromPrefab(dev, solo.EffectPrefab.Font)
local material = solo.Material.create(dev, effect)
local node = scene:createNode()
local meshRenderer = node:addComponent("MeshRenderer")
meshRenderer:setMesh(mesh)
meshRenderer:setMaterial(0, material)

function keepRunning()
    return not dev:isQuitRequested() and
           not dev:isWindowCloseRequested() and
           not dev:isKeyPressed(solo.KeyCode.Escape, true)
end

function update()
    scene:visit(function(cmp) cmp:update() end)

    local lifetime = dev:getLifetime()
        
    local color = camera:getClearColor()
    color.x = math.abs(math.sin(lifetime))

    camera:setClearColor(color)
end

function render()
    camera:renderFrame(function(ctx)
        scene:visit(function(cmp) cmp:render(ctx) end)
    end)
end

function run()
    while keepRunning() do
        dev:update(function()
            loader:update()
                loader:update()
                physics:update()
                update()
                renderer:renderFrame(function()
                    render()
                end)
        end)
    end
end

function runSafe()
    local _, err = select(1, pcall(run))
    if err then
        logger:logError(err)
    end
end

runSafe()
