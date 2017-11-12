--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

package.path = "../../src/demos/demo1/?.lua;" .. package.path
package.path = "../../src/demos/common/?.lua;" .. package.path

require "Common"

local dev = sl.device
local physics = dev:getPhysics()
local renderer = dev:getRenderer()
local logger = dev:getLogger()
local fs = dev:getFileSystem()
local scene = sl.Scene.create(dev)

local createMainCamera = require "MainCamera"
local postProcessors = require "PostProcessors"
local createRotator = require "Rotator"
local createLookAt = require "LookAt"
local createTransparentQuad = require "TransparentQuad"
local createMonitorQuad = require "MonitorQuad"
local createSkybox = require "Skybox"
local createTimeLabel = require "TimeLabel"
local createFloor = require "Floor"
local createDynamicQuad = require "DynamicQuad"
local createCheckerBox = require "CheckerBox"
local createLoadedMesh = require "LoadedMesh"
local createOffscreenCamera = require "OffscreenCamera"
local createEffects = require "Effects"
local createAxesAttacher = require "Axes"
local createSpawner = require "Spawner"
local createSpawnedObjectTargeter = require "SpawnedObjectTargeter"

local knownTags = {
    skybox = 1 << 1,
    transparent = 1 << 2,
    monitor = 1 << 3,
    postProcessor = 1 << 4
}

local cubeMesh = sl.Mesh.createFromPrefab(dev, sl.MeshPrefab.Cube)
local quadMesh = sl.Mesh.createFromPrefab(dev, sl.MeshPrefab.Quad)
local effects = createEffects(dev)
local offscreenCamera, offscreenCameraTex = createOffscreenCamera(dev, scene)
local mainCamera, mainCameraNode = createMainCamera(dev, scene)
mainCameraNode:addScriptComponent(createSpawnedObjectTargeter(dev:getPhysics()))
mainCameraNode:addScriptComponent(createSpawner(dev, cubeMesh, effects.color))
createSkybox(dev, scene, knownTags.skybox)
createCheckerBox(dev, scene, effects, cubeMesh)
createDynamicQuad(dev, scene, effects)
createTimeLabel(dev, scene, knownTags.transparent)
createLoadedMesh(dev, scene, effects)
createFloor(dev, scene, effects, cubeMesh)
local monitorQuad = createMonitorQuad(dev, scene, effects, offscreenCameraTex, quadMesh, knownTags.monitor)
local transparentQuad = createTransparentQuad(dev, scene, effects, quadMesh, knownTags.transparent)
local attachAxes = createAxesAttacher(dev, effects)
attachAxes(monitorQuad)
attachAxes(transparentQuad)

local originAxesNode = scene:createNode()
attachAxes(originAxesNode)

--- 

local run = function()
    local keepRunning = function()
        return (
            not dev:isQuitRequested() and
            not dev:isWindowCloseRequested() and
            not dev:isKeyPressed(sl.KeyCode.Escape, true)
            ) or dev:hasActiveBackgroundJobs()
    end

    local detachPostProcessor = function()
        if pp then
            pp:detach()
            pp = nil
        end
    end

    local update = function()
        scene:visit(function(cmp) cmp:update() end)

        if dev:isKeyPressed(sl.KeyCode.Digit1, true) then
            detachPostProcessor()
            pp = postProcessors.create1(mainCamera, knownTags.postProcessor, effects)
        end

        if dev:isKeyPressed(sl.KeyCode.Digit2, true) then
            detachPostProcessor()
            pp = postProcessors.create2(mainCamera, knownTags.postProcessor, effects)
        end

        if dev:isKeyPressed(sl.KeyCode.Digit3, true) then
            detachPostProcessor()
        end
    end

    local renderByTags = function(tags)
        scene:visitByTags(tags, function(cmp) cmp:render() end)
    end

    local render = function()
        offscreenCamera:renderFrame(function()
            renderByTags(knownTags.skybox)
            renderByTags(~(knownTags.skybox | knownTags.transparent | knownTags.monitor | knownTags.postProcessor))
            renderByTags(knownTags.transparent)
        end)

        mainCamera:renderFrame(function()
            renderByTags(knownTags.skybox)
            renderByTags(~(knownTags.skybox | knownTags.transparent | knownTags.postProcessor))
            renderByTags(knownTags.transparent)
        end)

        if pp then
            pp:apply()
        end
    end

    while keepRunning() do
        dev:update(function()
            physics:update()
            update()
            renderer:renderFrame(render)
        end)
    end
end

callSafe(run)
