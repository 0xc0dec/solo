--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

package.path = "../../src/demos/demo1/?.lua;" .. package.path
package.path = "../../src/demos/common/?.lua;" .. package.path

require "Common"

function demo()
    local dev = sl.device
    local physics = dev:getPhysics()
    local renderer = dev:getRenderer()
    local logger = dev:getLogger()
    local fs = dev:getFileSystem()
    local scene = sl.Scene.create(dev)

    local assetCache = (require "AssetCache")(dev)
    local createMainCamera = require "MainCamera"
    local postProcessors = require "PostProcessors"
    local createTransparentQuad = require "TransparentQuad"
    local createMonitorQuad = require "MonitorQuad"
    local createSkybox = require "Skybox"
    local createTimeLabel = require "TimeLabel"
    local createFloor = require "Floor"
    local createDynamicQuad = require "DynamicQuad"
    local createCheckerBox = require "CheckerBox"
    local createLoadedMesh = require "LoadedMesh"
    local createOffscreenCamera = require "OffscreenCamera"
    local attachAxes = (require "Axes")(dev, assetCache)
    local createSpawner = require "Spawner"
    local createSpawnedObjectTargeter = require "SpawnedObjectTargeter"

    ---

    local knownTags = {
        skybox = 1 << 1,
        transparent = 1 << 2,
        monitor = 1 << 3,
        postProcessor = 1 << 4
    }

    local cubeMesh = sl.Mesh.createFromPrefab(dev, sl.MeshPrefab.Cube)
    local quadMesh = sl.Mesh.createFromPrefab(dev, sl.MeshPrefab.Quad)
    local offscreenCamera, offscreenCameraTex = createOffscreenCamera(dev, scene)

    local mainCamera, mainCameraNode = createMainCamera(dev, scene)
    mainCameraNode:addScriptComponent(createSpawnedObjectTargeter(dev:getPhysics()))
    mainCameraNode:addScriptComponent(createSpawner(dev, cubeMesh, assetCache))
    local mainCameraTransform = mainCameraNode:findComponent("Transform")
    mainCameraTransform:setLocalPosition(vec3(0, 5, 10))
    mainCameraTransform:lookAt(vec3(0, 0, 0), vec3(0, 1, 0))

    createSkybox(dev, scene, knownTags.skybox)
    createCheckerBox(dev, scene, assetCache, cubeMesh)
    createDynamicQuad(dev, scene, assetCache)
    createTimeLabel(dev, scene, knownTags.transparent)
    createLoadedMesh(dev, scene, assetCache)
    createFloor(dev, scene, assetCache, cubeMesh)

    local monitorQuad = createMonitorQuad(dev, scene, assetCache, offscreenCameraTex, quadMesh, knownTags.monitor)
    attachAxes(monitorQuad)

    local transparentQuad = createTransparentQuad(dev, scene, assetCache, quadMesh, knownTags.transparent)
    attachAxes(transparentQuad)

    local originNode = scene:createNode()
    attachAxes(originNode)

    --- 

    function run()
        function keepRunning()
            return (
                not dev:isQuitRequested() and
                not dev:isWindowCloseRequested() and
                not dev:isKeyPressed(sl.KeyCode.Escape, true)
                ) or dev:hasActiveBackgroundJobs()
        end

        function detachPostProcessor()
            if pp then
                pp:detach()
                pp = nil
            end
        end

        function updateCmp(cmp)
            cmp:update()
        end

        function renderCmp(cmp)
            cmp:render()
        end

        function update()
            scene:visit(updateCmp)

            if dev:isKeyPressed(sl.KeyCode.Digit1, true) then
                detachPostProcessor()
                pp = postProcessors.create1(mainCamera, knownTags.postProcessor, assetCache)
            end

            if dev:isKeyPressed(sl.KeyCode.Digit2, true) then
                detachPostProcessor()
                pp = postProcessors.create2(mainCamera, knownTags.postProcessor, assetCache)
            end

            if dev:isKeyPressed(sl.KeyCode.Digit3, true) then
                detachPostProcessor()
            end
        end

        function renderByTags(tags)
            scene:visitByTags(tags, renderCmp)
        end

        function renderOffscreenFrame()
            renderByTags(knownTags.skybox)
            renderByTags(~(knownTags.skybox | knownTags.transparent | knownTags.monitor | knownTags.postProcessor))
            renderByTags(knownTags.transparent)
        end

        function renderFrame()
            renderByTags(knownTags.skybox)
            renderByTags(~(knownTags.skybox | knownTags.transparent | knownTags.postProcessor))
            renderByTags(knownTags.transparent)
        end

        function render()
            offscreenCamera:renderFrame(renderOffscreenFrame)
            mainCamera:renderFrame(renderFrame)

            if pp then
                pp:apply()
            end
        end

        function updateAndRender()
            physics:update()
            update()
            renderer:renderFrame(render)
        end

        while keepRunning() do
            dev:update(updateAndRender)
        end
    end

    run()
end

callSafe(demo)