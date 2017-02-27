--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

dofile("../../src/demos/common/Common.lua")

collectgarbage("setpause", 100)

local getAssetPath = function(fileName)
    return "../../assets/" .. fileName
end

local dev = solo.device
local loader = dev:getAssetLoader()
local physics = dev:getPhysics()
local renderer = dev:getRenderer()
local logger = dev:getLogger()
local fs = dev:getFileSystem()
local scene = solo.Scene.create(dev)
local canvasSize = dev:getCanvasSize()

local postProcessors = dofile("../../src/demos/demo1/PostProcessors.lua")
local shaders = dofile("../../src/demos/demo1/Shaders.lua")(fs, getAssetPath)
local createMainCamera = dofile("../../src/demos/demo1/MainCamera.lua")
local createOffscreenCamera = dofile("../../src/demos/demo1/OffscreenCamera.lua")
local createSkybox = dofile("../../src/demos/demo1/Skybox.lua")
local createCheckerBox = dofile("../../src/demos/demo1/CheckerBox.lua")
local createDynamicQuad = dofile("../../src/demos/demo1/DynamicQuad.lua")
local createTransparentQuad = dofile("../../src/demos/demo1/TransparentQuad.lua")
local createMonitorQuad = dofile("../../src/demos/demo1/MonitorQuad.lua")
local createFloor = dofile("../../src/demos/demo1/Floor.lua")
local createMonkeyHead = dofile("../../src/demos/demo1/MonkeyHead.lua")
local createTimeLabel = dofile("../../src/demos/demo1/TimeLabel.lua")
local attachAxesMesh = dofile("../../src/demos/demo1/Axes.lua")

local knownTags = {
    skybox = 1 << 1,
    transparent = 1 << 2,
    monitor = 1 << 3,
    postProcessor = 1 << 4
}

local effects = {
    simpleTexture = solo.Effect.create(dev, shaders.vs.basic, shaders.fs.texture),
    color = solo.Effect.create(dev, shaders.vs.basic, shaders.fs.color)
}

local meshes = {
    cube = solo.Mesh.createFromPrefab(dev, solo.MeshPrefab.Cube),
    quad = solo.Mesh.createFromPrefab(dev, solo.MeshPrefab.Quad)
}

local loadTextureAsync = function(path, callback)
    loader:loadRectTextureAsync(path):done(function(tex)
        tex:generateMipmaps()
        tex:setFiltering(solo.TextureFiltering.LinearMipmapNearest)
        tex:setAnisotropyLevel(8)
        callback(tex)
    end)
end

local mainCamera = createMainCamera(dev, scene, physics, meshes, effects)
local offscreenCamera, offscreenCameraTex = createOffscreenCamera(dev, scene)
createSkybox(scene, loader, getAssetPath, knownTags.skybox)
createCheckerBox(dev, scene, shaders, meshes.cube)
createDynamicQuad(dev, scene, effects, loadTextureAsync, getAssetPath)
createTimeLabel(dev, scene, knownTags.transparent, fs:readBytes(getAssetPath("Aller.ttf")))

loadTextureAsync(getAssetPath("Cobblestone.png"), function(tex)
    loader:loadMeshAsync(getAssetPath("MonkeyHD.obj")):done(function(mesh)
        createMonkeyHead(dev, scene, shaders, tex, mesh)
    end)
    createFloor(dev, scene, effects, tex, meshes.cube)
end)

loader:loadMeshAsync(getAssetPath("Axes.obj")):done(function(axesMesh)
    loadTextureAsync(getAssetPath("Flammable.png"), function(tex)
        attachAxesMesh(dev, effects,
            createMonitorQuad(dev, scene, effects, offscreenCameraTex, meshes.quad, knownTags.monitor),
            axesMesh)
        attachAxesMesh(dev, effects,
            createTransparentQuad(dev, scene, effects, meshes.quad, knownTags.transparent, tex),
            axesMesh)
    end)
end)

local keepRunning = function()
    return not dev:isQuitRequested() and
           not dev:isWindowCloseRequested() and
           not dev:isKeyPressed(solo.KeyCode.Escape, true)
end

local detachPostProcessor = function()
    if pp then
        pp:detach()
        pp = nil
    end
end

local update = function()
    scene:visit(function(cmp) cmp:update() end)

    if dev:isKeyPressed(solo.KeyCode.Digit1, true) then
        detachPostProcessor()
        pp = postProcessors.create1(dev, mainCamera, knownTags.postProcessor, shaders)
    end

    if dev:isKeyPressed(solo.KeyCode.Digit2, true) then
        detachPostProcessor()
        pp = postProcessors.create2(dev, loader, mainCamera, knownTags.postProcessor, shaders, getAssetPath)
    end

    if dev:isKeyPressed(solo.KeyCode.Digit3, true) then
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
            loader:update()
                loader:update()
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
