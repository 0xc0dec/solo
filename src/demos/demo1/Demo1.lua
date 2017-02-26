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
local initMainCamera = dofile("../../src/demos/demo1/MainCamera.lua")
local initOffscreenCamera = dofile("../../src/demos/demo1/OffscreenCamera.lua")
local initSkybox = dofile("../../src/demos/demo1/Skybox.lua")
local initCheckerBox = dofile("../../src/demos/demo1/CheckerBox.lua")
local initDynamicQuad = dofile("../../src/demos/demo1/DynamicQuad.lua")
local initTransparentQuad = dofile("../../src/demos/demo1/TransparentQuad.lua")
local initMonitorQuad = dofile("../../src/demos/demo1/MonitorQuad.lua")
local initFloor = dofile("../../src/demos/demo1/Floor.lua")
local initMonkeyHead = dofile("../../src/demos/demo1/MonkeyHead.lua")
local initTimeLabel = dofile("../../src/demos/demo1/TimeLabel.lua")

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

local createColorMaterial = function(color)
    local mat = solo.Material.create(dev, effects.color)
    mat:setFaceCull(solo.FaceCull.All)
    mat:bindWorldViewProjectionMatrixParameter("worldViewProjMatrix")
    mat:setVector4Parameter("color", color)
    return mat
end

local materials = {
    red = createColorMaterial(vec4(1, 0, 0, 1)),
    green = createColorMaterial(vec4(0, 1, 0, 1)),
    blue = createColorMaterial(vec4(0, 0, 1, 1)),
    white = createColorMaterial(vec4(1, 1, 1, 1))
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

local attachAxesMesh = function(node, axesMesh)
    local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(axesMesh)
    renderer:setMaterial(0, materials.blue)
    renderer:setMaterial(1, materials.green)
    renderer:setMaterial(2, materials.white)
    renderer:setMaterial(3, materials.red)
end

local mainCamera = initMainCamera(dev, scene, physics, meshes, effects)
local offscreenCamera, offscreenCameraTex = initOffscreenCamera(dev, scene)
initSkybox(scene, loader, getAssetPath, knownTags.skybox)
initCheckerBox(dev, scene, shaders, meshes.cube)
initDynamicQuad(dev, scene, effects, loadTextureAsync, getAssetPath)
initTimeLabel(dev, fs, scene, knownTags.transparent, getAssetPath)

loadTextureAsync(getAssetPath("Cobblestone.png"), function(tex)
    loader:loadMeshAsync(getAssetPath("MonkeyHD.obj")):done(function(mesh)
        initMonkeyHead(dev, scene, shaders, tex, mesh)
    end)
    initFloor(dev, scene, effects, tex, meshes.cube)
end)

loader:loadMeshAsync(getAssetPath("Axes.obj")):done(function(axesMesh)
    loadTextureAsync(getAssetPath("Flammable.png"), function(tex)
        attachAxesMesh(
            initMonitorQuad(dev, scene, effects, offscreenCameraTex, meshes.quad, knownTags.monitor),
            axesMesh)
        attachAxesMesh(
            initTransparentQuad(dev, scene, effects, meshes.quad, knownTags.transparent, tex),
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

local renderByTags = function(tags, ctx)
    scene:visitByTags(tags, function(cmp) cmp:render(ctx) end)
end

local render = function()
    offscreenCamera:renderFrame(function(ctx)
        renderByTags(knownTags.skybox, ctx)
        renderByTags(~(knownTags.skybox | knownTags.transparent | knownTags.monitor | knownTags.postProcessor), ctx)
        renderByTags(knownTags.transparent, ctx)
    end)

    mainCamera:renderFrame(function(ctx)
        renderByTags(knownTags.skybox, ctx)
        renderByTags(~(knownTags.skybox | knownTags.transparent | knownTags.postProcessor), ctx)
        renderByTags(knownTags.transparent, ctx)
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
