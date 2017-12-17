--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

package.path = "../../src/demos/demo2/?.lua;" .. package.path
package.path = "../../src/demos/common/?.lua;" .. package.path

require "Common"

function demo()
    local dev = sl.device
    local physics = dev:getPhysics()
    local renderer = dev:getRenderer()
    local logger = dev:getLogger()
    local fs = dev:getFileSystem()
    local scene = sl.Scene.create(dev)

    local createRotator = require "Rotator"
    local createMainCamera = require "MainCamera"
    local createSkybox = require "Skybox"
    local assetCache = (require "AssetCache")()
    local attachAxes = (require "Axes")(assetCache)

    function createMaterial(t)
        local tex = t or sl.Texture2d.loadFromFile(dev, getAssetPath("textures/Cobblestone.png"))
        local effect = assetCache.getEffect("Texture")
        local material = sl.Material.create(dev, effect)
        material:setFaceCull(sl.FaceCull.None)
        material:bindParameter("matrices.wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)
        material:setTextureParameter("colorTex", tex)

        return material
    end

    function createOffscreenCamera()
        local canvasSize = dev:getCanvasSize()

        local tex = sl.Texture2d.createEmpty(
            dev,
            canvasSize.x, canvasSize.y,
            sl.TextureFormat.RGBA
        )
        tex:setFiltering(sl.TextureFiltering.Nearest)
        tex:setWrapping(sl.TextureWrapping.Clamp)

        local node = scene:createNode()

        local cam = node:addComponent("Camera")
        cam:setClearColor(vec4(1, 0, 1, 1))
        cam:setZNear(0.05)
        cam:setViewport(vec4(0, 0, canvasSize.x, canvasSize.y))

        local fb = sl.FrameBuffer.create(dev)
        fb:setAttachments({ tex })
        cam:setRenderTarget(fb)

        return cam, tex, node
    end

    function createCustomMesh(material, position)
        local mesh = sl.Mesh.create(dev)

        local layout = sl.VertexBufferLayout()
        layout:addAttribute(3, 0)
        local positions = {
            1, 1, 0,
            -1, 1, 0,
            -1, -1, 0,

            1, 1, 0,
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
            1.0, 1.0,

            0.0, 0.0,
            1.0, 1.0,
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

        node:addScriptComponent(createRotator("world", vec3(0, 1, 0), 2))
        
        local meshRenderer = node:addComponent("MeshRenderer")
        meshRenderer:setMesh(mesh)
        meshRenderer:setMaterial(0, material)

        if position then
            node:findComponent("Transform"):setLocalPosition(position)
        end
    end

    ---

    local tags = {
        skybox = 1 << 1,
    }

    local offscreenCamera, offscreenCameraTex, offscreenCameraNode = createOffscreenCamera()
    local offscreenCameraTransform = offscreenCameraNode:findComponent("Transform")
    offscreenCameraTransform:setLocalPosition(vec3(5, 5, 5))
    offscreenCameraTransform:lookAt(vec3(0, 0, 0), vec3(0, 1, 0))

    local camera, cameraNode = createMainCamera(scene)
    local cameraTransform = cameraNode:findComponent("Transform")
    cameraTransform:setLocalPosition(vec3(5, 6, 7))
    cameraTransform:lookAt(vec3(0, 0, 0), vec3(0, 1, 0))

    local material1 = createMaterial(offscreenCameraTex)
    local material2 = createMaterial()

    createSkybox(scene, tags.skybox)
    createCustomMesh(material1, vec3(-3, 3, 0))
    createMesh(material2, vec3(3, 3, 0))

    local rootNode = scene:createNode()
    attachAxes(rootNode)

    ---

    function keepRunning()
        return not dev:isQuitRequested() and
               not dev:isWindowCloseRequested() and
               not dev:isKeyPressed(sl.KeyCode.Escape, true)
    end

    function renderCmp(cmp)
        cmp:render()
    end

    function updateCmp(cmp)
        cmp:update()
    end

    function update()
        scene:visit(updateCmp)

        local lifetime = dev:getLifetime()
            
        local color = camera:getClearColor()
        color.x = math.abs(math.sin(lifetime))
        camera:setClearColor(color)
    end

    function renderFrame()
        scene:visitByTags(tags.skybox, renderCmp)
        scene:visitByTags(~tags.skybox, renderCmp)
    end

    function render()
        offscreenCamera:renderFrame(renderFrame)
        camera:renderFrame(renderFrame)
    end

    function updateAndRender()
        physics:update()
        update()
        renderer:renderFrame(render)
    end

    function run()
        while keepRunning() do
            dev:update(updateAndRender)
        end
    end

    run()
end

callSafe(demo)
