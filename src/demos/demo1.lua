--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

package.path = "../../src/demos/?.lua;" .. package.path

require "common"

function demo()
    local dev = sl.device
    local physics = dev:getPhysics()
    local fs = dev:getFileSystem()
    local scene = sl.Scene.create(dev)

    local createMainCamera = require "main-camera"
    local createSkybox = require "skybox"
    local assetCache = (require "asset-cache")()
    local attachAxes = (require "axes")(assetCache)
    local createFloor = require "floor"
    local createCheckerBox = require "checker-box"
    local createDynamicQuad = require "dynamic-quad"
    local createTransparentQuad = require "transparent-quad"
    local createLoadedMesh = require "loaded-mesh"
    local createSpawner = require "spawner"
    local createMonitorQuad = require "monitor-quad"
    local createOffscreenCamera = require "offscreen-camera"
    local createPostProcessor = require "post-processor"
    local createTracer = require "tracer"
    local createPostProcessorControlPanel = require "post-processor-control-panel"
    local tags = require "tags"

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

    local skybox = createSkybox(scene, assetCache)
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
        scene:visitByTags(skybox.tag, renderCmp)
        scene:visitByTags(~(tags.monitor | skybox.tag | tags.transparent | tags.postProcessorStep), renderCmp)
        scene:visitByTags(tags.transparent, renderCmp)
    end

    function renderMainCamera()
        scene:visitByTags(skybox.tag, renderCmp)
        scene:visitByTags(~(skybox.tag | tags.transparent | tags.postProcessorStep), renderCmp)
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
