--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

package.path = "../../src/demos/demo2/?.lua;" .. package.path
package.path = "../../src/demos/common/?.lua;" .. package.path

require "Common"

local dev = sl.device
local physics = dev:getPhysics()
local renderer = dev:getRenderer()
local logger = dev:getLogger()
local fs = dev:getFileSystem()
local scene = sl.Scene.create(dev)

local createMainCamera = require "MainCamera"
local assetCache = (require "AssetCache")(dev)
local attachAxes = (require "Axes")(dev, assetCache)

function createMaterial()
    local tex = sl.Texture2d.loadFromFile(dev, getAssetPath("textures/Cobblestone.png"))
    local effect = assetCache.getEffect("Test")
    local material = sl.Material.create(dev, effect)
    
    material:bindParameter("matrices.wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)
    material:setTextureParameter("colorTex", tex)
    material:setVector4Parameter("test.v4", vec4(0.1, 0.2, 0.3, 0.4))
    material:setVector3Parameter("test.v3", vec3(0.5, 0.6, 0.7))
    material:setVector2Parameter("test.v2", vec2(0.8, 0.9))

    return material
end

function createOffscreenCamera()
    local canvasSize = dev:getCanvasSize()

    local tex = sl.Texture2d.createEmpty(
        dev,
        math.floor(canvasSize.x / 8.0), math.floor(canvasSize.y / 8.0),
        sl.TextureFormat.RGBA
    )
    tex:setFiltering(sl.TextureFiltering.Nearest)
    tex:setWrapping(sl.TextureWrapping.Clamp)

    local node = scene:createNode()
    node:findComponent("Transform"):setLocalPosition(vec3(5, 5, 5))

    local cam = node:addComponent("Camera")
    cam:setClearColor(vec4(1, 0, 1, 1))
    cam:setNear(0.05)
    cam:setViewport(vec4(0, 0, canvasSize.x / 8, canvasSize.y / 8))

    local fb = sl.FrameBuffer.create(dev)
    fb:setAttachments({ tex })
    cam:setRenderTarget(fb)

    return cam
end

function createCustomMesh(material, position)
    local mesh = sl.Mesh.create(dev)

    local layout = sl.VertexBufferLayout()
    layout:addAttribute(3, 0)
    local positions = {
        0.5, 1, 0,
        -0.5, 1, 0,
        -1, -1, 0,

        0.5, 1, 0,
        -1, -1, 0,
        1, -1, 0,
    }
    mesh:addVertexBuffer(layout, positions, 6)

    layout = sl.VertexBufferLayout()
    layout:addAttribute(3, 1)
    local normals = {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,

        0, 0, 0,
        0, 0, 0,
        0, 0, 0
    }
    mesh:addVertexBuffer(layout, normals, 6)

    layout = sl.VertexBufferLayout()
    layout:addAttribute(2, 2)
    local texCoords = {
        0.0, 0.0,
        0.0, 1.0,
        1.0, 0.0,

        0.0, 0.0,
        0.0, 1.0,
        1.0, 0.0
    }
    mesh:addVertexBuffer(layout, texCoords, 6)
    mesh:addPart({0, 1, 2, 3, 4, 5}, 6)

    local node = scene:createNode()

    local meshRenderer = node:addComponent("MeshRenderer")
    meshRenderer:setMesh(mesh)
    meshRenderer:setMaterial(0, material)

    if position then
        node:findComponent("Transform"):setLocalPosition(position)
    end
end

function createMesh(material, position)
    local mesh = sl.Mesh.loadFromFile(dev, getAssetPath("meshes/Teapot.obj"))
    local node = scene:createNode()
    local meshRenderer = node:addComponent("MeshRenderer")
    meshRenderer:setMesh(mesh)
    meshRenderer:setMaterial(0, material)

    if position then
        node:findComponent("Transform"):setLocalPosition(position)
    end
end

---

local material = createMaterial()

local camera, cameraNode = createMainCamera(dev, scene)
local cameraTransform = cameraNode:findComponent("Transform")
cameraTransform:setLocalPosition(vec3(5, 6, 7))
cameraTransform:lookAt(vec3(0, 0, 0), vec3(0, 1, 0))

local offscreenCamera = createOffscreenCamera()

createCustomMesh(material, vec3(-2, 0, 0))
createMesh(material, vec3(2, 0, 0))

local rootNode = scene:createNode()
attachAxes(rootNode)

---

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

    material:setFloatParameter("test.f", math.abs(math.sin(lifetime)) * 4)
end

function render()
    offscreenCamera:renderFrame(function(ctx)
        scene:visit(function(cmp) cmp:render(ctx) end)
    end)

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

callSafe(run)
