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
    local createFloor = require "Floor"
    local createCheckerBox = require "CheckerBox"
    local createDynamicQuad = require "DynamicQuad"
    local createTransparentQuad = require "TransparentQuad"
    local postProcessors = require "PostProcessors"
    local createLoadedMesh = require "LoadedMesh"
    local createTimeLabel = require "TimeLabel"
    local createSpawner = require "Spawner"
    local createMonitorQuad = require "MonitorQuad"
    local createOffscreenCamera = require "OffscreenCamera"

    ---

    local tags = {
        skybox = 1 << 1,
        transparent = 1 << 2,
        monitor = 1 << 3,
        postProcessor = 1 << 4
    }

    local layers = {
        skybox = 1, -- default is 16, so this render before
        transparent = 20 -- default is 16, so this render after
    }

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
    offscreenCamera:setTagMask(~tags.monitor)

    local mainCamera, mainCameraNode = createMainCamera(scene)
    local mainCameraTransform = mainCameraNode:findComponent("Transform")
    mainCameraTransform:setLocalPosition(vec3(0, 5, 10))
    mainCameraTransform:lookAt(vec3(0, 0, 0), vec3(0, 1, 0))
    mainCameraNode:addScriptComponent(createSpawner(cubeMesh, assetCache))
    mainCamera:setOrder(1)

    createSkybox(scene, layers.skybox)
    createCheckerBox(scene, assetCache, cubeMesh)
    createFloor(scene, assetCache, cubeMesh, commonTextures.cobbleStone)
    createDynamicQuad(scene, assetCache)
    createLoadedMesh(scene, assetCache, commonTextures.cobbleStone)
    createTimeLabel(scene, layers.transparent)

    local monitorQuadParent = createMonitorQuad(scene, assetCache, offscreenCameraTex, quadMesh, tags.monitor)
    attachAxes(monitorQuadParent)

    local transparentQuad = createTransparentQuad(scene, assetCache, quadMesh, layers.transparent)
    attachAxes(transparentQuad)

    local rootNode = scene:createNode()
    attachAxes(rootNode)

    ---

    function detachPostProcessor()
        if pp then
            pp:detach()
            pp = nil
        end
    end

    function keepRunning()
        return not dev:isQuitRequested() and
               not dev:isWindowCloseRequested() and
               not dev:isKeyPressed(sl.KeyCode.Escape, true)
    end

    function update()
        if dev:isKeyPressed(sl.KeyCode.Digit1, true) then
            detachPostProcessor()
            pp = postProcessors.create1(mainCamera, tags.postProcessor, assetCache)
        end

        if dev:isKeyPressed(sl.KeyCode.Digit2, true) then
            detachPostProcessor()
            pp = postProcessors.create2(mainCamera, tags.postProcessor, assetCache)
        end

        if dev:isKeyPressed(sl.KeyCode.Digit3, true) then
            detachPostProcessor()
        end
    end

    function run()
        while keepRunning() do
            dev:update(scene)
            collectgarbage("collect")
        end
    end

    run()
end

callSafe(demo)
