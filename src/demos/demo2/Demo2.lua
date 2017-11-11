--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

package.path = "../../src/demos/demo2/?.lua;" .. package.path
package.path = "../../src/demos/common/?.lua;" .. package.path

require "Common"

dev = sl.device
physics = dev:getPhysics()
renderer = dev:getRenderer()
logger = dev:getLogger()
scene = sl.Scene.create(dev)

function createMaterial()
    local tex = sl.Texture2d.loadFromFile(dev, getAssetPath("textures/Cobblestone.png"))
    local effect = sl.Effect.loadFromFiles(dev,
        getAssetPath("shaders/vulkan/Test.vert.spv"),
        getAssetPath("shaders/vulkan/Test.frag.spv"))
    local material = sl.Material.create(dev, effect)
    
    material:bindParameter("matrices.wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)
    material:setTextureParameter("colorTex", tex)
    material:setVector4Parameter("test.v4", vec4(0.1, 0.2, 0.3, 0.4))
    material:setVector3Parameter("test.v3", vec3(0.5, 0.6, 0.7))
    material:setVector2Parameter("test.v2", vec2(0.8, 0.9))

    return material
end

function createMainCamera()
    local node = scene:createNode()
    
    local cam = node:addComponent("Camera")
    cam:setClearColor(vec4(0.0, 0.6, 0.6, 1.0))
    cam:setNear(0.05)

    node:addComponent("Spectator")

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

function createAxesAttacher()
    local mesh = sl.Mesh.loadFromFile(dev, getAssetPath("meshes/Axes.obj"))
    local effect = sl.Effect.loadFromFiles(dev,
        getAssetPath("shaders/vulkan/Color.vert.spv"),
        getAssetPath("shaders/vulkan/Color.frag.spv"))

    local createColorMaterial = function(color)
        local mat = sl.Material.create(dev, effect)
        mat:setFaceCull(sl.FaceCull.All)
        mat:bindParameter("matrices.wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)
        mat:setVector4Parameter("variables.color", color)
        return mat
    end

    local materials = {
        red = createColorMaterial(vec4(1, 0, 0, 1)),
        green = createColorMaterial(vec4(0, 1, 0, 1)),
        blue = createColorMaterial(vec4(0, 0, 1, 1)),
        white = createColorMaterial(vec4(1, 1, 1, 1))
    }

    return function(node)
        local renderer = node:addComponent("MeshRenderer")
        renderer:setMesh(mesh)
        renderer:setMaterial(0, materials.blue)
        renderer:setMaterial(1, materials.green)
        renderer:setMaterial(2, materials.white)
        renderer:setMaterial(3, materials.red)
    end
end

local material = createMaterial()
local camera = createMainCamera()
createCustomMesh(material, vec3(-2, 0, 0))
createMesh(material, vec3(2, 0, 0))

local axesNode = scene:createNode()
local attachAxes = createAxesAttacher()
attachAxes(axesNode)

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
