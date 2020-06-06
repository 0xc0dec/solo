--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

sl.includeDir("../../../src/demos/lua/demo1")
sl.includeDir("../../../src/demos/lua/common")

require "common"

function demo()
    local fs = sl.device:fileSystem()
    local physics = sl.device:physics()
    local renderer = sl.device:renderer()
    local scene = sl.Scene.empty(sl.device)
    local ui = sl.device:debugInterface()

    local tags = require "tags"
    local createRotator = require "rotator"
    local createMainCamera = require "main-camera"
    local createSkybox = require "skybox"
    local assetCache = (require "asset-cache")()
    local createPostProcessor = require "post-processor"
    local createPostProcessorControlPanel = require "post-processor-control-panel"
    local createTracer = require "tracer"
    local createSpawner = require "spawner"
    local createCheckerBox = require "checker-box"
    local createDynamicQuad = require "dynamic-quad"
    local attachAxes = (require "axes")(assetCache)
    local createLookAt = require "look-at"
    local createHighlighter = require "highlighter"
    local createLightCamera = require "light-camera"
    local createGrabber = require "grabber"

    ---

    function createMesh(meshPath, material, onMeshLoad)
        local node = scene:createNode()
    
        local renderer = node:addComponent("MeshRenderer")
        renderer:setDefaultMaterial(material)
    
        local transform = node:findComponent("Transform")
    
        local layout = sl.VertexBufferLayout()
        layout:addAttribute(sl.VertexAttributeUsage.Position)
        layout:addAttribute(sl.VertexAttributeUsage.Normal)
        layout:addAttribute(sl.VertexAttributeUsage.Tangent)
        layout:addAttribute(sl.VertexAttributeUsage.TexCoord)
        sl.Mesh.fromFileAsync(sl.device, assetPath(meshPath), layout)
            :done(function(mesh)
                renderer:setMesh(mesh)
                onMeshLoad(mesh)
            end)

        return {
            node = node,
            transform = transform,
            renderer = renderer
        }
    end

    function createTracker(parentTransform, pos, target)
        local node = scene:createNode()
        local transform = node:findComponent("Transform")
        transform:setParent(parentTransform)
        transform:setLocalPosition(pos)
        node:addScriptComponent(createLookAt(target))
        attachAxes(node)
    end

    function createStaticColliderMesh(meshPath, material)
        local body

        local context = createMesh(meshPath, material,
            function(mesh)
                local col = sl.StaticMeshCollider.fromMesh(mesh)
                body:setCollider(col)
            end)
        
        local params = sl.RigidBodyParams()
        params.mass = 0
        params.friction = 0.5
        
        body = context.node:addComponent("RigidBody", params)
        body:setStatic(true)
        
        return context
    end

    function createBackdrop(material)
        return createStaticColliderMesh("meshes/backdrop.obj", material)
    end

    function createHouse(material)
        return createStaticColliderMesh('meshes/house.obj', material)
    end

    function createTeapot(material)
        local context = createStaticColliderMesh("meshes/teapot.obj", material)
        context.transform:setLocalPosition(vec3(3, 0, -3))
    end

    function createShadowedMaterial(lightCam)
        local eff = assetCache.effect("shadowed")
        local mat = sl.Material.fromEffect(sl.device, eff)
        mat:setFaceCull(sl.FaceCull.None)
        mat:bindParameter("uniforms:wvp", sl.ParameterBinding.WorldViewProjectionMatrix)
        mat:bindParameter("uniforms:world", sl.ParameterBinding.WorldMatrix)
        mat:bindParameter("uniforms:camPos", sl.ParameterBinding.CameraWorldPosition)
        mat:setTextureParameter("colorMap", assetCache.textures.texture1.color)
        mat:setTextureParameter("normalMap", assetCache.textures.texture1.normal)
        mat:setTextureParameter("shadowMap", lightCam.depthTex)
        mat:bindMatrixParameter("uniforms:lightVp", function() return lightCam.camera:viewProjectionMatrix() end)
        mat:bindVector3Parameter("uniforms:lightPos", function() return lightCam.transform:worldPosition() end)
        mat:setFloatParameter('variables:highlighted', 0)
        return mat
    end

    function createSpectatorCamera(shadowedMat)
        local camera, node = createMainCamera(scene)
        node:findComponent("Transform"):setLocalPosition(vec3(10, 10, -5))
        node:findComponent("Transform"):lookAt(vec3(0, 2, 0), vec3(0, 1, 0))
        node:addScriptComponent(createTracer(sl.device, scene, physics, assetCache))
        node:addScriptComponent(createHighlighter(assetCache, physics))
        node:addScriptComponent(createSpawner(assetCache, shadowedMat))
        node:addScriptComponent(createGrabber())

        return {
            camera = camera,
            node = node
        }
    end

    function createAxes()
        local rootNode = scene:createNode()
        local rootTransform = rootNode:findComponent("Transform")
        attachAxes(rootNode)
        rootNode:addScriptComponent(createRotator("world", vec3(0, 1, 0), 1))

        createTracker(rootTransform, vec3(4, 4, 4), vec3(3, 0, -3))
        createTracker(rootTransform, vec3(-4, 4, 4), vec3(-3, 1, 3))
        createTracker(rootTransform, vec3(4, 4, -4), vec3(3, 1, 3))
    end

    ---

    local lightCam = createLightCamera(scene)
    local shadowedMat = createShadowedMaterial(lightCam)
    
    local spawnedObjMat = shadowedMat:clone()
    spawnedObjMat:setTextureParameter('colorMap', assetCache.textures.texture2.color)
    spawnedObjMat:setTextureParameter('normalMap', assetCache.textures.texture2.normal)
    local mainCamera = createSpectatorCamera(spawnedObjMat)

    local postProcessor = createPostProcessor(assetCache, mainCamera.camera)
    local ppControlPanel = createPostProcessorControlPanel(assetCache, mainCamera.node, postProcessor)
    ppControlPanel.transform:setLocalPosition(vec3(-7, 0, -5))
    ppControlPanel.transform:rotateByAxisAngle(vec3(0, 1, 0),
        sl.Radians.fromRawDegrees(90), sl.TransformSpace.World)

    mainCamera.node:addScriptComponent(ppControlPanel.cmp)

    local skybox = createSkybox(scene, assetCache)
    
    local dynamicQuad = createDynamicQuad(scene, assetCache)
    dynamicQuad.transform:setLocalPosition(vec3(3, 1, 3))

    local checkerBox = createCheckerBox(scene, assetCache)
    checkerBox.transform:setLocalPosition(vec3(-3, 1, 3))

    createAxes()
    createTeapot(shadowedMat)
    createBackdrop(shadowedMat)
    createHouse(shadowedMat)

    ---

    function shouldStop()
        return sl.device:isQuitRequested() or
               sl.device:isWindowCloseRequested() or
               sl.device:isKeyPressed(sl.KeyCode.Escape, true)
    end

    function renderLightCamFrame()
        scene:render(~(skybox.tag | tags.postProcessorStep))
    end

    function renderMainCamFrame()
        scene:render(skybox.tag)
        scene:render(~(skybox.tag | tags.postProcessorStep | tags.transparent))
        scene:render(tags.transparent)
    end

    function renderUi()
        local windowCfg = sl.WindowConfig()
        windowCfg.title = 'Window title'
        windowCfg.position = vec2(10, 10)
        windowCfg.movable = false
        windowCfg.decoration = false
        windowCfg.alpha = 0.5

        local canvasSize = sl.device:canvasSize()
        local dpiIndepCanvasSize = sl.device:dpiIndependentCanvasSize()
        local canvasSizeNote1 = 'Canvas size: (' .. canvasSize.x .. ', ' .. canvasSize.y .. ')\n'
        local canvasSizeNote2 = 'DPI-independent canvas size: (' .. dpiIndepCanvasSize.x .. ', ' .. dpiIndepCanvasSize.y .. ')\n'

        ui:renderWindow(windowCfg, function()
            local text = canvasSizeNote1 .. canvasSizeNote2 .. [[
Controls:
    Hold RMB - rotate camera
    W, A, S, D, Q, E - move camera
    Space - spawn a box
    G - grab/drop an object
]]
            ui:renderText(text)
        end)
    end

    function update()
        scene:update()
        lightCam.camera:renderFrame(renderLightCamFrame)
        mainCamera.camera:renderFrame(renderMainCamFrame)
        -- postProcessor:apply()
        renderUi()
    end

    function run()
        while not shouldStop() do
            sl.device:update(update)
        end
    end

    run()
end

callSafe(demo)
