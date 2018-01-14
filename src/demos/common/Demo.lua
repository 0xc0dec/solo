--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

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
    local createFloor = require "Floor"
    local createCheckerBox = require "CheckerBox"
    local createDynamicQuad = require "DynamicQuad"
    local createTransparentQuad = require "TransparentQuad"
    local createLoadedMesh = require "LoadedMesh"
    local createTimeLabel = require "TimeLabel"
    local createSpawner = require "Spawner"
    local createMonitorQuad = require "MonitorQuad"
    local createOffscreenCamera = require "OffscreenCamera"
    local createPostProcessor = require "PostProcessor"
    local createTracer = require "Tracer"
    local tags = require "Tags"

    ---

    local cubeMesh = sl.Mesh.createFromPrefab(dev, sl.MeshPrefab.Cube)
    local quadMesh = sl.Mesh.createFromPrefab(dev, sl.MeshPrefab.Quad)

    function loadCommonTextures()
        local cobbleStone = sl.Texture2d.loadFromFile(sl.device, getAssetPath("textures/Cobblestone.png"), true)
        cobbleStone:setAnisotropyLevel(16)

        return {
            cobbleStone = cobbleStone
        }
    end

    local commonTextures = loadCommonTextures()

    local offscreenCamera, offscreenCameraTex = createOffscreenCamera(scene)
    offscreenCamera:setOrder(0)
    offscreenCamera:setTagMask(~(tags.monitor | tags.allPostProcessorSteps))

    local mainCamera, mainCameraNode = createMainCamera(scene, physics)
    local mainCameraTransform = mainCameraNode:findComponent("Transform")
    mainCameraTransform:setLocalPosition(vec3(0, 5, 10))
    mainCameraTransform:lookAt(vec3(0, 0, 0), vec3(0, 1, 0))
    mainCameraNode:addScriptComponent(createTracer(physics))
    mainCameraNode:addScriptComponent(createSpawner(cubeMesh, assetCache))
    mainCameraNode:addScriptComponent(createPostProcessor(assetCache))
    mainCamera:setOrder(1)
    mainCamera:setTagMask(~tags.allPostProcessorSteps)

    createSkybox(scene)
    createCheckerBox(scene, assetCache, cubeMesh)
    createFloor(scene, assetCache, cubeMesh, commonTextures.cobbleStone)
    createDynamicQuad(scene, assetCache)
    createLoadedMesh(scene, assetCache, commonTextures.cobbleStone)
    createTimeLabel(scene)

    local monitorQuadParent = createMonitorQuad(scene, assetCache, offscreenCameraTex, quadMesh)
    attachAxes(monitorQuadParent)

    local transparentQuad = createTransparentQuad(scene, assetCache, quadMesh)
    attachAxes(transparentQuad)

    local rootNode = scene:createNode()
    attachAxes(rootNode)

    ---

    function shouldStop()
        return dev:isQuitRequested() or
               dev:isWindowCloseRequested() or
               dev:isKeyPressed(sl.KeyCode.Escape, true)
    end

    function run()
        while not shouldStop() do
            dev:update(scene)
            collectgarbage("collect")
        end
    end

    run()
end

callSafe(demo)
