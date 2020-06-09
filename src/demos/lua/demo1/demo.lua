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
    local createSpectatorCamera = require 'spectator-camera'
    local createSkybox = require 'skybox'
    local assetCache = require 'asset-cache'
    local createPostProcessors = require 'post-processor'
    local createTracer = require 'tracer'
    local createSpawner = require 'spawner'
    local createCheckerBox = require 'checker-box'
    local createDynamicQuad = require 'dynamic-quad'
    local createAxes = (require 'axes')(scene)
    local createLightCamera = require 'light-camera'
    local createGrabber = require 'grabber'
    local createStaticMesh = (require 'static-mesh')(scene)
    local createLabel = require 'label'

    ---

    function createBackdrop(material)
        return createStaticMesh('meshes/backdrop.obj', material)
    end

    function createTeapot(material)
        local teapot = createStaticMesh('meshes/teapot.obj', material)
        teapot.transform:setLocalPosition(vec3(0, 0, -6))
        createLabel('Static mesh', teapot.node, vec3(1.3, 3.5, 0), sl.Radians.fromRawDegrees(180))
        return teapot
    end

    function createHouse(srcMaterial)
        local mat = srcMaterial:clone()
        mat:setTextureParameter('colorMap', assetCache.textures.house.color)
        mat:setTextureParameter('normalMap', assetCache.textures.house.normal)
        return createStaticMesh('meshes/house.obj', mat)
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

    function createMainCamera()
        local camera = createSpectatorCamera(scene)
        local transform = camera.node:findComponent('Transform')
        transform:setLocalPosition(vec3(3, 14, -15))
        transform:lookAt(vec3(-2, 0, 0), vec3(0, 1, 0))
        camera.node:addScriptComponent(createTracer(scene))
        camera.node:addScriptComponent(createGrabber())
        
        return {
            camera = camera.camera,
            node = camera.node,
            transform = transform
        }
    end

    function addSpawner(cameraNode, shadowedMat)
        local spawnedObjMat = shadowedMat:clone()
        spawnedObjMat:setTextureParameter('colorMap', assetCache.textures.texture2.color)
        spawnedObjMat:setTextureParameter('normalMap', assetCache.textures.texture2.normal)
        cameraNode:addScriptComponent(createSpawner(spawnedObjMat))
    end

    ---

    local lightCam = createLightCamera(scene)
    local shadowedMat = createShadowedMaterial(lightCam)
    local mainCamera = createMainCamera()

    addSpawner(mainCamera.node, shadowedMat)

    local postProcessors = createPostProcessors(mainCamera.camera)
    local currentPostProcessor = nil

    local dynamicQuad = createDynamicQuad(scene)
    dynamicQuad.transform:setLocalPosition(vec3(3, 1, -2))
    createLabel('Dynamic mesh', dynamicQuad.node, vec3(1.3, 1.5, 0), sl.Radians.fromRawDegrees(180))

    local checkerBox = createCheckerBox(scene)
    checkerBox.transform:setLocalPosition(vec3(-4, 1, -2))

    local skybox = createSkybox(scene)

    createAxes()
    createBackdrop(shadowedMat)
    
    local house = createHouse(shadowedMat)
    house.transform:rotateByAxisAngle(vec3(0, 1, 0), sl.Radians.fromRawDegrees(-90), sl.TransformSpace.World)
    house.transform:setLocalPosition(vec3(0, 0, 4))

    local teapot = createTeapot(shadowedMat)
    
    ---

    function shouldStop()
        return sl.device:isQuitRequested() or
               sl.device:isWindowCloseRequested() or
               sl.device:isKeyPressed(sl.KeyCode.Escape, true)
    end

    function renderLightCamFrame()
        scene:render(~(skybox.tag | tags.postProcessor))
    end

    function renderMainCamFrame()
        scene:render(skybox.tag)
        scene:render(~(skybox.tag | tags.postProcessor | tags.transparent))
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
        local canvasSizeNote = string.format('Canvas size: (%d, %d)\n', canvasSize.x, canvasSize.y)
        
        local camPosition = mainCamera.transform:worldPosition()
        local camPositionNote = string.format('Camera position: (%.2f, %.2f, %.2f)\n',
            camPosition.x, camPosition.y, camPosition.y)

        local tracer = mainCamera.node:findScriptComponent(sl.cmpId('Tracer'))
        local tracerTarget = tracer.hitPoint
        local tracerTargetNote = string.format('Trace target: (%s)\n',
            tracerTarget and string.format('%.2f, %.2f, %.2f', tracerTarget.x, tracerTarget.y, tracerTarget.z) or 'none')
        
            local controlsNote = [[
Controls:
    Hold RMB - rotate camera
    W, A, S, D, Q, E - move camera
    Space - spawn a box
    G - grab/drop an object
    1, 2 - switch to post-processing type 1 or 2
    3 - clear post-processing
]]

        ui:renderWindow(windowCfg, function()
            local text = canvasSizeNote .. camPositionNote .. tracerTargetNote .. controlsNote
            ui:renderText(text)
        end)
    end

    function updatePostProcessor()
        if sl.device:isKeyPressed(sl.KeyCode.Digit1, true) then
            currentPostProcessor = postProcessors[1]
        elseif sl.device:isKeyPressed(sl.KeyCode.Digit2, true) then
            currentPostProcessor = postProcessors[2]
        elseif sl.device:isKeyPressed(sl.KeyCode.Digit3, true) then
            if currentPostProcessor then
                currentPostProcessor.cleanup()
            end
            currentPostProcessor = nil
        end
    end

    function update()
        scene:update()
        updatePostProcessor()

        if currentPostProcessor then
            currentPostProcessor.begin()
        end

        lightCam.camera:renderFrame(renderLightCamFrame)
        mainCamera.camera:renderFrame(renderMainCamFrame)

        if currentPostProcessor then
            currentPostProcessor.render()
        end

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
