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
local canvasSize = dev:getCanvasSize()

local postProcessors = require "PostProcessors"
local createEffects = require "Effects"
local createMainCamera = require "MainCamera"
local createOffscreenCamera = require "OffscreenCamera"
local createSkybox = require "Skybox"
local createCheckerBox = require "CheckerBox"
local createDynamicQuad = require "DynamicQuad"
local createTransparentQuad = require "TransparentQuad"
local createMonitorQuad = require "MonitorQuad"
local createRotatingMesh = require "RotatingMesh"
local attachAxesMesh = require "Axes"

local loadTexture2d = function(path)
    local tex = sl.Texture2d.loadFromFile(dev, path)
    tex:generateMipmaps()
    tex:setFiltering(sl.TextureFiltering.LinearMipmapNearest)
    tex:setAnisotropyLevel(8)
    return tex
end

local createFloor = function(effects, tex, cubeMesh)
    local material = sl.Material.create(sl.device, effects.simpleTexture)
    material:setFaceCull(sl.FaceCull.All)
    material:bindParameter("worldViewProjMatrix", sl.BindParameterSemantics.WorldViewProjectionMatrix)
    material:setTextureParameter("mainTex", tex)

    local node = scene:createNode()
    
    local transform = node:findComponent("Transform")
    transform:setLocalScale(vec3(10, 0.1, 10))
    transform:setLocalPosition(vec3(0, -2, 0))

    local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(cubeMesh)
    renderer:setMaterial(0, material)

    local params = sl.RigidBodyConstructionParameters()
    params.mass = 0
    params.friction = 0.5
    local body = node:addComponent("RigidBody", params)
    body:setCollider(sl.BoxCollider.create(vec3(1, 1, 1)))
end

local createTimeLabel = function(tag, fontPath)
    local createUpdater = function()
        local textureWidth = 1024
        local textureHeight = 1024
        local lineHeight = 60
        local renderer

        return {
            typeId = sl.getCmpId("TimeLabelUpdater"),

            init = function(self)
                local firstChar = string.byte(" ")
                local charCount = string.byte("~") - string.byte(" ")
                local font = sl.Font.loadFromFile(sl.device, fontPath,
                    lineHeight, textureWidth,
                    textureHeight, firstChar, charCount, 2, 2)

                renderer = self.node:addComponent("FontRenderer")
                renderer:setFont(font)
            end,

            update = function()
                renderer:setText(os.date("Now is %H:%M:%S"))
            end
        }
    end

    local node = scene:createNode()
    node:addScriptComponent(createUpdater())
    node:findComponent("FontRenderer"):setTag(tag)
    node:findComponent("Transform"):setLocalScale(vec3(0.02, 0.02, 1))
    node:findComponent("Transform"):setLocalPosition(vec3(-3, 0, 4))
end

local effects = createEffects()

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

local mainCamera = createMainCamera(scene, physics, meshes, effects)
local offscreenCamera, offscreenCameraTex = createOffscreenCamera(scene)
createSkybox(scene, knownTags.skybox)
createCheckerBox(scene, effects, meshes.cube)
createDynamicQuad(scene, effects, loadTexture2d)
createTimeLabel(knownTags.transparent, getAssetPath("fonts/Aller.ttf"))

local stoneTex = loadTexture2d(getAssetPath("textures/Cobblestone.png"))
local monkeyHeadMesh = sl.Mesh.loadFromFile(dev, getAssetPath("meshes/Teapot.obj"))
createRotatingMesh(scene, effects, stoneTex, monkeyHeadMesh)
createFloor(effects, stoneTex, meshes.cube)

local axesMesh = sl.Mesh.loadFromFile(dev, getAssetPath("meshes/Axes.obj"))
local logoTex = loadTexture2d(getAssetPath("textures/Flammable.png"))

attachAxesMesh(effects,
    createMonitorQuad(scene, effects, offscreenCameraTex, meshes.quad, knownTags.monitor),
    axesMesh)
attachAxesMesh(effects,
    createTransparentQuad(scene, effects, meshes.quad, knownTags.transparent, logoTex),
    axesMesh)

local run = function()
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
