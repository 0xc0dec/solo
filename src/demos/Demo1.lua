--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

package.path = "../../src/demos/?.lua;" .. package.path

require "Common"

function demo()
    local dev = sl.device
    local physics = dev:getPhysics()
    local fs = dev:getFileSystem()
    local scene = sl.Scene.create(dev)

    local createMainCamera = require "MainCamera"
    local createSkybox = require "Skybox"
    local assetCache = (require "AssetCache")()
    local attachAxes = (require "Axes")(assetCache)
    local createFloor = require "Floor"
    local createCheckerBox = require "CheckerBox"
    local createDynamicQuad = require "DynamicQuad"
    local createTransparentQuad = require "TransparentQuad"
    local createLoadedMesh = require "LoadedMesh"
    local createSpawner = require "Spawner"
    local createMonitorQuad = require "MonitorQuad"
    local createOffscreenCamera = require "OffscreenCamera"
    local createPostProcessor = require "PostProcessor"
    local createTracer = require "Tracer"
    local createPostProcessorControlPanel = require "PostProcessorControlPanel"
    local tags = require "Tags"

    ---

    local offscreenCamera, offscreenCameraTex = createOffscreenCamera(scene)

    local mainCamera, mainCameraNode = createMainCamera(scene)
    local mainCameraTransform = mainCameraNode:findComponent("Transform")
    mainCameraTransform:setLocalPosition(vec3(0, 5, 10))
    mainCameraTransform:lookAt(vec3(0, 0, 0), vec3(0, 1, 0))
    mainCameraNode:addScriptComponent(createTracer(physics))
    mainCameraNode:addScriptComponent(createSpawner(assetCache))
    
    local postProcessor = createPostProcessor(assetCache, mainCamera)
    mainCameraNode:addScriptComponent(createPostProcessorControlPanel(assetCache, mainCameraNode, postProcessor))

    createSkybox(scene)
    createCheckerBox(scene, assetCache)
    createFloor(scene, assetCache)
    createDynamicQuad(scene, assetCache)
    createLoadedMesh(scene, assetCache)

    local monitorQuadParent = createMonitorQuad(scene, assetCache, offscreenCameraTex)
    attachAxes(monitorQuadParent)

    local transparentQuad = createTransparentQuad(scene, assetCache)
    attachAxes(transparentQuad)

    local rootNode = scene:createNode()
    attachAxes(rootNode)

    ---

    function shouldStop()
        return dev:isQuitRequested() or
               dev:isWindowCloseRequested() or
               dev:isKeyPressed(sl.KeyCode.Escape, true)
    end

    function renderCmp(cmp)
        cmp:render()
    end

    function updateCmp(cmp)
        cmp:update()
    end

    function renderOffscreenCamera()
        scene:visitByTags(tags.skybox, renderCmp)
        scene:visitByTags(~(tags.monitor | tags.skybox | tags.transparent | tags.postProcessorStep), renderCmp)
        scene:visitByTags(tags.transparent, renderCmp)
    end

    function renderMainCamera()
        scene:visitByTags(tags.skybox, renderCmp)
        scene:visitByTags(~(tags.skybox | tags.transparent | tags.postProcessorStep), renderCmp)
        scene:visitByTags(tags.transparent, renderCmp)
    end

    function update()
        scene:visit(updateCmp)
        offscreenCamera:renderFrame(renderOffscreenCamera)
        mainCamera:renderFrame(renderMainCamera)
        postProcessor:apply()
    end

    function run()
        while not shouldStop() do
            dev:update(update)
            collectgarbage("collect")
        end
    end

    run()
end

callSafe(demo)
