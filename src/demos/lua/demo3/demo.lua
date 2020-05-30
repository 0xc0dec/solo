--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

sl.includeDir("../../../src/demos/lua/common")
sl.includeDir("../../../src/demos/lua/demo1")

require "common"

function demo()
    local dev = sl.device
    local physics = dev:physics()
    local fs = dev:fileSystem()
    local scene = sl.Scene.empty(dev)
    
    local tags = require "tags"
    local createMainCamera = require "main-camera"
    local createSkybox = require "skybox"
    local assetCache = (require "asset-cache")()

    ---------------------

    function createSpectatorCamera()
        local camera, node = createMainCamera(scene)
        node:findComponent("Transform"):setLocalPosition(vec3(10, 10, -5))
        node:findComponent("Transform"):lookAt(vec3(0, 2, 0), vec3(0, 1, 0))

        return {
            camera = camera,
            node = node
        }
    end

    function createMesh(meshPath, material)
        local node = scene:createNode()
    
        local renderer = node:addComponent("MeshRenderer")
        renderer:setDefaultMaterial(material)
    
        local transform = node:findComponent("Transform")
    
        local layout = sl.VertexBufferLayout()
        layout:addAttribute(sl.VertexAttributeUsage.Position)
        layout:addAttribute(sl.VertexAttributeUsage.Normal)
        layout:addAttribute(sl.VertexAttributeUsage.TexCoord)
        sl.Mesh.fromFileAsync(sl.device, assetPath(meshPath), layout)
            :done(function(mesh) renderer:setMesh(mesh) end)

        return {
            node = node,
            transform = transform,
            renderer = renderer
        }
    end

    function createColorMaterial(color)
        local effect = assetCache.getEffect("color")
        local m = sl.Material.fromEffect(sl.device, effect)
        m:setPolygonMode(sl.PolygonMode.Wireframe)
        m:setFaceCull(sl.FaceCull.None)
        m:bindParameter("matrices:wvp", sl.ParameterBinding.WorldViewProjectionMatrix)
        m:setVector4Parameter("variables:color", color)
        return m
    end

    ----------------------

    local colorMat = createColorMaterial(vec4(0, 1, 0, 1))
    local mainCamera = createSpectatorCamera()
    local skybox = createSkybox(scene, assetCache)

    createMesh("meshes/sponza.dae", colorMat)

    ----------------------

    function shouldStop()
        return sl.device:isQuitRequested() or
               sl.device:isWindowCloseRequested() or
               sl.device:isKeyPressed(sl.KeyCode.Escape, true)
    end

    function renderCmp(cmp)
        cmp:render()
    end

    function updateCmp(cmp)
        cmp:update()
    end

    function renderMainCam()
        scene:visitByTags(skybox.tag, renderCmp)
        scene:visitByTags(~skybox.tag, renderCmp)
    end

    function update()
        scene:visit(updateCmp)
        mainCamera.camera:renderFrame(renderMainCam)
    end

    function run()
        while not shouldStop() do
            sl.device:update(update)
            collectgarbage("collect")
        end
    end

    run()
end

callSafe(demo)
