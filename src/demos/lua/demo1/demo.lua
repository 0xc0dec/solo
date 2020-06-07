--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

sl.includeDir('../../../src/demos/lua/demo1')
sl.includeDir('../../../src/demos/lua/common')

require 'common'

function demo()
    local scene = sl.Scene.empty(sl.device)
    local ui = sl.device:debugInterface()

    local tags = require 'tags'
    local createRotator = require 'rotator'
    local createMainCamera = require 'main-camera'
    local createSkybox = require 'skybox'
    local assetCache = (require 'asset-cache')()
    local createPostProcessor = require 'post-processor'
    local createPostProcessorControlPanel = require 'post-processor-control-panel'
    local createTracer = require 'tracer'
    local createSpawner = require 'spawner'
    local createCheckerBox = require 'checker-box'
    local createDynamicQuad = require 'dynamic-quad'
    local createAxes = (require 'axes')(assetCache, scene)
    local createLookAt = require 'look-at'
    local createLightCamera = require 'light-camera'
    local createGrabber = require 'grabber'
    local createStaticMesh = (require 'static-mesh')(scene)

    ---

    function createBackdrop(material)
        return createStaticMesh('meshes/backdrop.obj', material)
    end

    function createTeapot(material)
        local result = createStaticMesh('meshes/teapot.obj', material)
        result.transform:setLocalPosition(vec3(3, 0, -3))
    end

    function createHouse(srcMaterial)
        local mat = srcMaterial:clone()
        mat:setTextureParameter('colorMap', assetCache.textures.house.color)
        mat:setTextureParameter('normalMap', assetCache.textures.house.normal)
        local house = createStaticMesh('meshes/house.obj', mat)
        house.transform:rotateByAxisAngle(vec3(0, 1, 0), sl.Radians.fromRawDegrees(-90), sl.TransformSpace.World)
    end

    function createShadowedMaterial(lightCam)
        local eff = assetCache.effect('shadowed')
        local mat = sl.Material.fromEffect(sl.device, eff)
        mat:setFaceCull(sl.FaceCull.Back)
        mat:bindParameter('uniforms:wvp', sl.ParameterBinding.WorldViewProjectionMatrix)
        mat:bindParameter('uniforms:world', sl.ParameterBinding.WorldMatrix)
        mat:bindParameter('uniforms:camPos', sl.ParameterBinding.CameraWorldPosition)
        mat:setTextureParameter('colorMap', assetCache.textures.texture1.color)
        mat:setTextureParameter('normalMap', assetCache.textures.texture1.normal)
        mat:setTextureParameter('shadowMap', lightCam.depthTex)
        mat:bindMatrixParameter('uniforms:lightVp', function() return lightCam.camera:viewProjectionMatrix() end)
        mat:bindVector3Parameter('uniforms:lightDir', function() return lightCam.transform:worldForward() end)
        return mat
    end

    function createSpectatorCamera()
        local camera, node = createMainCamera(scene)
        node:findComponent('Transform'):setLocalPosition(vec3(10, 10, -5))
        node:findComponent('Transform'):lookAt(vec3(0, 2, 0), vec3(0, 1, 0))
        node:addScriptComponent(createTracer(scene, assetCache))
        node:addScriptComponent(createGrabber())
        
        return {
            camera = camera,
            node = node
        }
    end

    function addSpawner(cameraNode, shadowedMat)
        local spawnedObjMat = shadowedMat:clone()
        spawnedObjMat:setTextureParameter('colorMap', assetCache.textures.texture2.color)
        spawnedObjMat:setTextureParameter('normalMap', assetCache.textures.texture2.normal)
        cameraNode:addScriptComponent(createSpawner(assetCache, spawnedObjMat))
    end

    ---

    local lightCam = createLightCamera(scene)
    local shadowedMat = createShadowedMaterial(lightCam)
    local mainCamera = createSpectatorCamera()

    addSpawner(mainCamera.node, shadowedMat)

    local postProcessor = createPostProcessor(assetCache, mainCamera.camera)
    local ppControlPanel = createPostProcessorControlPanel(assetCache, mainCamera.node, postProcessor)
    ppControlPanel.transform:setLocalPosition(vec3(-7, 0, -5))
    ppControlPanel.transform:rotateByAxisAngle(vec3(0, 1, 0), sl.Radians.fromRawDegrees(90), sl.TransformSpace.World)

    mainCamera.node:addScriptComponent(ppControlPanel.cmp)

    local dynamicQuad = createDynamicQuad(scene, assetCache)
    dynamicQuad.transform:setLocalPosition(vec3(3, 1, 3))

    local checkerBox = createCheckerBox(scene, assetCache)
    checkerBox.transform:setLocalPosition(vec3(-3, 1, 3))

    local skybox = createSkybox(scene, assetCache)
    
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
        postProcessor:apply()
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
