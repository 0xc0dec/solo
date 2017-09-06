--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

-- TODO avoid using globals in functions

package.path = "../../src/demos/demo2/?.lua;../../src/demos/common/?.lua;" .. package.path

require "Common"

dev = sl.device
physics = dev:getPhysics()
renderer = dev:getRenderer()
logger = dev:getLogger()
scene = sl.Scene.create(dev)

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

local mesh = sl.Mesh.create(dev)

local layout = sl.VertexBufferLayout()
layout:addAttribute(2, 0)
local positions = {
	0.9, 0.9,
    -0.9, 0.9,
    -0.9, -0.9,

    0.9, 0.8,
    -0.8, -0.9,
    0.9, -0.9
}
mesh:addVertexBuffer(layout, positions, 6)

layout = sl.VertexBufferLayout()
layout:addAttribute(3, 1)
local colors = {
	0.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    1.0, 0.0, 1.0,

    0.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    1.0, 0.0, 1.0
}
mesh:addVertexBuffer(layout, colors, 6)

mesh:addPart({ 0, 1, 2, 3, 4, 5 }, 6)

local tex = sl.RectTexture.create(dev, 100, 100, sl.TextureFormat.RGBA)
local effect = sl.Effect.loadFromFiles(dev, getAssetPath("Triangle.vert.spv"), getAssetPath("Triangle.frag.spv"))
local material = sl.Material.create(dev, effect)
material:setFloatParameter("0:0", 12.3)
material:setFloatArrayParameter("0:1", {1, 2, 3})
material:setTextureParameter("1", tex)

camera = initMainCamera()

local node = scene:createNode()
local meshRenderer = node:addComponent("MeshRenderer")
meshRenderer:setMesh(mesh)
meshRenderer:setMaterial(0, material)

function keepRunning()
    return not dev:isQuitRequested() and
           not dev:isWindowCloseRequested() and
           not dev:isKeyPressed(sl.KeyCode.Escape, true)
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
