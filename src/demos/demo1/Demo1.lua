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
local createRotator = require "Rotator"
local createScreenshoter = require "Screenshoter"
local createSpawnedObject = require "SpawnedObject"
local createLookAt = require "LookAt"

local loadTexture2d = function(path)
    local tex = sl.Texture2d.loadFromFile(dev, path)
    tex:generateMipmaps()
    tex:setFiltering(sl.TextureFiltering.LinearMipmapNearest)
    tex:setAnisotropyLevel(8)
    return tex
end

local createSpawnedObjectTargeter = function()
    return {
        typeId = sl.getCmpId("SpawnedObjectTargeter"),

        init = function(self)
            self.transform = self.node:findComponent("Transform")
        end,

        update = function(self)
            local to = self.transform:getWorldPosition() + self.transform:getLocalForward() * 100
            local hitResults = sl.device:getPhysics():castRayAll(self.transform:getWorldPosition(), to)

            for i, hit in ipairs(hitResults) do
                local obj = hit.body:getNode():findScriptComponent(sl.getCmpId("SpawnedObject"))
                if obj then
                    obj:setActive()
                end
            end
        end
    }
end

local createSpawner = function(mesh, effect)
    return {
        typeId = sl.getCmpId("Spawner"),

        init = function(self)
            self.transform = self.node:findComponent("Transform")
            self.scene = self.node:getScene()
        end,

        update = function(self)
            if sl.device:isKeyPressed(sl.KeyCode.Space, true) then
                local initialPos = self.transform:getLocalPosition() + self.transform:getLocalForward() * 2
                local initialRotation = self.transform:getLocalRotation()
                self.scene:createNode():addScriptComponent(
                    createSpawnedObject(effect, mesh, initialPos, initialRotation))
            end
        end
    }
end

local createTransparentQuad = function(effects, quadMesh, tag, tex)
    tex:setWrapping(sl.TextureWrapping.Clamp)

    local material = sl.Material.create(sl.device, effects.simpleTexture)
    material:setFaceCull(sl.FaceCull.All)
    material:bindParameter("worldViewProjMatrix", sl.BindParameterSemantics.WorldViewProjectionMatrix)
    material:setTextureParameter("mainTex", tex)
    material:setBlend(true)
    material:setDepthTest(true)
    material:setDepthWrite(false)

    local parent = scene:createNode()
    parent:findComponent("Transform"):setLocalPosition(vec3(5, 0, 0))
    parent:addScriptComponent(createRotator("world", vec3(0, 1, 0)))

    local node = scene:createNode()
    node:addScriptComponent(createRotator("local", vec3(1, 0, 0)))
    node:findComponent("Transform"):setParent(parent:findComponent("Transform"))
    node:findComponent("Transform"):setLocalPosition(vec3(2, 0, 0))

    local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(quadMesh)
    renderer:setMaterial(0, material)
    renderer:setTag(tag)

    return parent
end

local attachAxesMesh = function(effects, node, mesh)
	-- TODO cache materials
	local createColorMaterial = function(color)
	    local mat = sl.Material.create(sl.device, effects.color)
	    mat:setFaceCull(sl.FaceCull.All)
	    mat:bindParameter("worldViewProjMatrix", sl.BindParameterSemantics.WorldViewProjectionMatrix)
	    mat:setVector4Parameter("color", color)
	    return mat
	end

	local materials = {
	    red = createColorMaterial(vec4(1, 0, 0, 1)),
	    green = createColorMaterial(vec4(0, 1, 0, 1)),
	    blue = createColorMaterial(vec4(0, 0, 1, 1)),
	    white = createColorMaterial(vec4(1, 1, 1, 1))
	}

	local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(mesh)
    renderer:setMaterial(0, materials.blue)
    renderer:setMaterial(1, materials.green)
    renderer:setMaterial(2, materials.white)
    renderer:setMaterial(3, materials.red)
end


local createMonitorQuad = function(effects, offscreenCameraTex, quadMesh, tag)
	local canvasSize = sl.device:getCanvasSize()

    local material = sl.Material.create(sl.device, effects.simpleTexture)
    material:setFaceCull(sl.FaceCull.All)
    material:bindParameter("worldViewProjMatrix", sl.BindParameterSemantics.WorldViewProjectionMatrix)
    material:setTextureParameter("mainTex", offscreenCameraTex)

    local parent = scene:createNode()
    parent:findComponent("Transform"):setLocalPosition(vec3(-2, 2, -2))
    parent:addScriptComponent(createRotator("world", vec3(0, 1, 0)))

    local node = scene:createNode()
    node:addScriptComponent(createLookAt(vec3(0, 0, 0)))

    local transform = node:findComponent("Transform")
    transform:setParent(parent:findComponent("Transform"))
    transform:setLocalPosition(vec3(5, 2, -5))
    transform:setLocalScale(vec3(5, 5 * canvasSize.y / canvasSize.x, 1))
    
    local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(quadMesh)
    renderer:setMaterial(0, material)
    renderer:setTag(tag)

    return parent
end

local createSkybox = function(tag)
    local getImagePath = function(fileName) return getAssetPath("textures/skyboxes/deep-space/") .. fileName end
    local tex = sl.CubeTexture.loadFromFaceFiles(sl.device,
        getImagePath("Front.png"), getImagePath("Back.png"), getImagePath("Left.png"),
        getImagePath("Right.png"), getImagePath("Top.png"), getImagePath("Bottom.png")
    )
    tex:setWrapping(sl.TextureWrapping.Clamp)
    tex:setFiltering(sl.TextureFiltering.Linear)
    local node = scene:createNode()
    local renderer = node:addComponent("SkyboxRenderer")
    renderer:setTexture(tex)
    renderer:setTag(tag)
end

local createMainCamera = function(meshes, effects)
    local node = scene:createNode()
        
    local t = node:findComponent("Transform")
    t:setLocalPosition(vec3(0, 5, 10))
    t:lookAt(vec3(0, 0, 0), vec3(0, 1, 0))
    
    node:addComponent("Spectator")
    node:addScriptComponent(createScreenshoter("Demo1.bmp"))
    node:addScriptComponent(createSpawnedObjectTargeter(physics))
    node:addScriptComponent(createSpawner(meshes.cube, effects.color))

    local cam = node:addComponent("Camera")
    cam:setClearColor(vec4(0.0, 0.6, 0.6, 1.0))
    cam:setNear(0.05)

    return cam
end

local createEffects = function()
    local shaderFiles = {
        vs = {
            basic = getAssetPath("shaders/gl/Basic.vert.glsl"),
            passThrough = getAssetPath("shaders/gl/PassThrough.vert.glsl"),
            basicLighting = getAssetPath("shaders/gl/BasicLighting.vert.glsl")
        },

        fs = {
            checker = getAssetPath("shaders/gl/Checker.frag.glsl"),
            textureWithLighting = getAssetPath("shaders/gl/TextureWithLighting.frag.glsl"),
            grayscale = getAssetPath("shaders/gl/Grayscale.frag.glsl"),
            saturate = getAssetPath("shaders/gl/Saturate.frag.glsl"),
            verticalBlur = getAssetPath("shaders/gl/VerticalBlur.frag.glsl"),
            horizontalBlur = getAssetPath("shaders/gl/HorizontalBlur.frag.glsl"),
            texture = getAssetPath("shaders/gl/Texture.frag.glsl"),
            color = getAssetPath("shaders/gl/Color.frag.glsl"),
            stitches = getAssetPath("shaders/gl/Stitches.frag.glsl")
        }
    }

    return {
        simpleTexture = sl.Effect.loadFromFiles(sl.device, shaderFiles.vs.basic, shaderFiles.fs.texture),
        color = sl.Effect.loadFromFiles(sl.device, shaderFiles.vs.basic, shaderFiles.fs.color),
        checker = sl.Effect.loadFromFiles(sl.device, shaderFiles.vs.basic, shaderFiles.fs.checker),
        basicLighting = sl.Effect.loadFromFiles(sl.device, shaderFiles.vs.basicLighting, shaderFiles.fs.textureWithLighting),
        grayscale = sl.Effect.loadFromFiles(sl.device, shaderFiles.vs.passThrough, shaderFiles.fs.grayscale),
        saturate = sl.Effect.loadFromFiles(sl.device, shaderFiles.vs.passThrough, shaderFiles.fs.saturate),
        verticalBlur = sl.Effect.loadFromFiles(sl.device, shaderFiles.vs.passThrough, shaderFiles.fs.verticalBlur),
        horizontalBlur = sl.Effect.loadFromFiles(sl.device, shaderFiles.vs.passThrough, shaderFiles.fs.horizontalBlur),
        stitches = sl.Effect.loadFromFiles(sl.device, shaderFiles.vs.passThrough, shaderFiles.fs.stitches)
    }
end

local createDynamicQuad = function(effects, loadTexture)
    local createUpdater = function(data, mesh)
        local time = 0

        return {
            typeId = sl.getCmpId("DynamicQuadUpdater"),

            update = function()
                time = time + 2 * sl.device:getTimeDelta()
                local offset = 0.5 * math.sin(time)
                data[3] = offset
                data[8] = -offset
                data[13] = offset
                data[18] = -offset
                mesh:updateDynamicVertexBuffer(0, 0, data, 4)
            end
        }
    end

    local tex = loadTexture(getAssetPath("textures/Bricks.jpg"))
    tex:setWrapping(sl.TextureWrapping.Clamp)

    local layout = sl.VertexBufferLayout()
    layout:addAttribute(3, 0)
    layout:addAttribute(2, 1)

    local data = {
        -1, -1, 0,    0, 0,
        -1,  1, 0,    0, 1,
        1,  1, 0,    1, 1,
        1, -1, 0,    1, 0
    }

    local indices = {
        0, 1, 2,
        0, 2, 3
    }

    local mesh = sl.Mesh.create(sl.device)
    mesh:addDynamicVertexBuffer(layout, data, 4)
    mesh:addPart(indices, 6)
    mesh:setPrimitiveType(sl.PrimitiveType.Triangles)

    local material = sl.Material.create(sl.device, effects.simpleTexture)
    material:setFaceCull(sl.FaceCull.All)
    material:bindParameter("worldViewProjMatrix", sl.BindParameterSemantics.WorldViewProjectionMatrix)
    material:setTextureParameter("mainTex", tex)

    local node = scene:createNode()
    node:findComponent("Transform"):setLocalPosition(vec3(0, 0, -5))
    local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(mesh)
    renderer:setMaterial(0, material)

    node:addScriptComponent(createUpdater(data, mesh))
end

local createCheckerBox = function(effects, cubeMesh)
    local material = sl.Material.create(sl.device, effects.checker)
    material:setFaceCull(sl.FaceCull.All)
    material:bindParameter("worldViewProjMatrix", sl.BindParameterSemantics.WorldViewProjectionMatrix)
    material:setVector4Parameter("color", vec4(1, 1, 0, 1))

    local node = scene:createNode()
    node:addComponent("MeshRenderer"):setMesh(cubeMesh)
    node:findComponent("MeshRenderer"):setMaterial(0, material)
    node:findComponent("Transform"):setLocalPosition(vec3(-5, 0, 0))
    node:addScriptComponent(createRotator("world", vec3(0, 1, 0)))
end

local createRotatingMesh = function(effects, tex, mesh)
    local material = sl.Material.create(sl.device, effects.basicLighting)
    material:setFaceCull(sl.FaceCull.All)
    material:bindParameter("worldViewProjMatrix", sl.BindParameterSemantics.WorldViewProjectionMatrix)
    material:bindParameter("invTransposedWorldMatrix", sl.BindParameterSemantics.InverseTransposedWorldMatrix)
    material:setTextureParameter("mainTex", tex)

    local node = scene:createNode()
    local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(mesh)
    renderer:setMaterial(0, material)
    node:findComponent("Transform"):setLocalPosition(vec3(0, 0, 0))
    node:addScriptComponent(createRotator("local", vec3(1, 0, 0)))
end

local createOffscreenCamera = function()
	local canvasSize = sl.device:getCanvasSize()

    local tex = sl.Texture2d.createEmpty(sl.device,
        math.floor(canvasSize.x / 8.0), math.floor(canvasSize.y / 8.0), sl.TextureFormat.RGB)
    tex:setFiltering(sl.TextureFiltering.Nearest)
    tex:setWrapping(sl.TextureWrapping.Clamp)

    local node = scene:createNode()
    node:findComponent("Transform"):setLocalPosition(vec3(0, 0, 10))

    local cam = node:addComponent("Camera")
    cam:setClearColor(vec4(1, 0, 1, 1))
    cam:setNear(0.05)
    cam:setViewport(vec4(0, 0, canvasSize.x / 8, canvasSize.y / 8))

    local fb = sl.FrameBuffer.create(sl.device)
    fb:setAttachments({ tex })
    cam:setRenderTarget(fb)

    return cam, tex
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

local mainCamera = createMainCamera(meshes, effects)
local offscreenCamera, offscreenCameraTex = createOffscreenCamera()
createSkybox(knownTags.skybox)
createCheckerBox(effects, meshes.cube)
createDynamicQuad(effects, loadTexture2d)
createTimeLabel(knownTags.transparent, getAssetPath("fonts/Aller.ttf"))

local stoneTex = loadTexture2d(getAssetPath("textures/Cobblestone.png"))
local monkeyHeadMesh = sl.Mesh.loadFromFile(dev, getAssetPath("meshes/Teapot.obj"))
createRotatingMesh(effects, stoneTex, monkeyHeadMesh)
createFloor(effects, stoneTex, meshes.cube)

local axesMesh = sl.Mesh.loadFromFile(dev, getAssetPath("meshes/Axes.obj"))
local logoTex = loadTexture2d(getAssetPath("textures/Flammable.png"))

attachAxesMesh(effects,
    createMonitorQuad(effects, offscreenCameraTex, meshes.quad, knownTags.monitor),
    axesMesh)
attachAxesMesh(effects,
    createTransparentQuad(effects, meshes.quad, knownTags.transparent, logoTex),
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
