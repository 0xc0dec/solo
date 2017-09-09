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
layout:addAttribute(2, 1)
local texCoords = {
	0.0, 0.0,
    0.0, 1.0,
    1.0, 0.0,

    0.0, 0.0,
    0.0, 1.0,
    1.0, 0.0
}
mesh:addVertexBuffer(layout, texCoords, 6)

mesh:addPart({ 0, 1, 2, 3, 4, 5 }, 6)

local tex = sl.Texture2d.loadFromFile(dev, getAssetPath("Cobblestone.png"))
local effect = sl.Effect.loadFromFiles(dev, getAssetPath("Triangle.vert.spv"), getAssetPath("Triangle.frag.spv"))
local material = sl.Material.create(dev, effect)
local m = sl.Matrix()
material:setMatrixParameter("matrices.test", m)
material:setTextureParameter("colorTex", tex)
material:setVector4Parameter("test.v4", vec4(0.1, 0.2, 0.3, 0.4))
material:setVector3Parameter("test.v3", vec3(0.5, 0.6, 0.7))
material:setVector2Parameter("test.v2", vec2(0.8, 0.9))
material:setFloatParameter("test.f", 1)

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
