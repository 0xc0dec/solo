--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

package.path = "../../src/demos/demo1/?.lua;../../src/demos/common/?.lua;" .. package.path

require "Common"

collectgarbage("setpause", 100)

local dev = sl.device
local physics = dev:getPhysics()
local renderer = dev:getRenderer()
local logger = dev:getLogger()
local fs = dev:getFileSystem()
local scene = sl.Scene.create(dev)
local canvasSize = dev:getCanvasSize()

local postProcessors = require "PostProcessors"
local effects = (require "Effects")()
local createMainCamera = require "MainCamera"
local createOffscreenCamera = require "OffscreenCamera"
local createSkybox = require "Skybox"
local createCheckerBox = require "CheckerBox"
local createDynamicQuad = require "DynamicQuad"
local createTransparentQuad = require "TransparentQuad"
local createMonitorQuad = require "MonitorQuad"
local createFloor = require "Floor"
local createMonkeyHead = require "MonkeyHead"
local createTimeLabel = require "TimeLabel"
local attachAxesMesh = require "Axes"

local knownTags = {
    skybox = 1 << 1,
    transparent = 1 << 2,
    monitor = 1 << 3,
    postProcessor = 1 << 4
}

local meshes = {
    cube = sl.Mesh.createFromPrefab(dev, sl.MeshPrefab.Cube),
    quad = sl.Mesh.createFromPrefab(dev, sl.MeshPrefab.Quad)
}

local loadTexture = function(path)
    local tex = sl.Texture.loadRectFromFile(dev, path)
    tex:generateMipmaps()
    tex:setFiltering(sl.TextureFiltering.LinearMipmapNearest)
    tex:setAnisotropyLevel(8)
    return tex
end

local mainCamera = createMainCamera(scene, physics, meshes, effects)
local offscreenCamera, offscreenCameraTex = createOffscreenCamera(scene)
createSkybox(scene, knownTags.skybox)
createCheckerBox(scene, effects, meshes.cube)
createDynamicQuad(scene, effects, loadTexture)
createTimeLabel(scene, knownTags.transparent, fs:readBytes(getAssetPath("Aller.ttf")))

local stoneTex = loadTexture(getAssetPath("Cobblestone.png"))
local monkeyHeadMesh = sl.Mesh.loadFromFile(dev, getAssetPath("MonkeyHD.obj"))
createMonkeyHead(scene, effects, stoneTex, monkeyHeadMesh)
createFloor(scene, effects, stoneTex, meshes.cube)

local axesMesh = sl.Mesh.loadFromFile(dev, getAssetPath("Axes.obj"))
local logoTex = loadTexture(getAssetPath("Flammable.png"))

attachAxesMesh(effects,
    createMonitorQuad(scene, effects, offscreenCameraTex, meshes.quad, knownTags.monitor),
    axesMesh)
attachAxesMesh(effects,
    createTransparentQuad(scene, effects, meshes.quad, knownTags.transparent, logoTex),
    axesMesh)

local keepRunning = function()
    return not dev:isQuitRequested() and
           not dev:isWindowCloseRequested() and
           not dev:isKeyPressed(sl.KeyCode.Escape, true)
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

local run = function()
    while keepRunning() do
        dev:update(function()
            physics:update()
            update()
            renderer:renderFrame(render)
        end)
    end
end

local callSafe = function(f)
    local _, err = select(1, pcall(f))
    if err then
        logger:logError(err)
    end
end

callSafe(run)
