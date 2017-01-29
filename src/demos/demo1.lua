-- TODO avoid using globals in functions

dev = solo.device
loader = dev:getAssetLoader()
physics = dev:getPhysics()
renderer = dev:getRenderer()
logger = dev:getLogger()
fs = dev:getFileSystem()
scene = solo.Scene.create(dev)
canvasSize = dev:getCanvasSize()

skyboxTag = 2
transparentTag = 4
monitorQuadTag = 8
postProcessorTag = 16

shaders = dofile("../../src/demos/shaders.lua")

effects = {
    simpleTexture = solo.Effect.create(dev, shaders.vs.basic, shaders.fs.texture),
    color = solo.Effect.create(dev, shaders.vs.basic, shaders.fs.color)
}

function createColorMaterial(color)
    local mat = solo.Material.create(dev, effects.color)
    mat:setFaceCull(solo.FaceCull.All)
    mat:bindWorldViewProjectionMatrixParameter("worldViewProjMatrix")
    mat:setVector4Parameter("color", color)
    return mat
end

materials = {
    red = createColorMaterial(solo.Vector4(1, 0, 0, 1)),
    green = createColorMaterial(solo.Vector4(0, 1, 0, 1)),
    blue = createColorMaterial(solo.Vector4(0, 0, 1, 1)),
    white = createColorMaterial(solo.Vector4(1, 1, 1, 1))
}

meshes = {
    cube = solo.Mesh.createFromPrefab(dev, solo.MeshPrefab.Cube),
    quad = solo.Mesh.createFromPrefab(dev, solo.MeshPrefab.Quad)
}

function initMainCamera()
    local node = scene:createNode()
        
    local t = node:findComponent("Transform")
    t:setLocalPosition(solo.Vector3(0, 5, 10))
    t:lookAt(solo.Vector3(0, 0, 0), solo.Vector3(0, 1, 0))
    
    -- TODO
    -- node:addComponent<Screenshoter>("demo1.bmp");
    node:addComponent("Spectator")
    -- node:addComponent<SpawnedObjectTargeter>();
    -- node:addComponent<Spawner>(cubeMesh);

    local cam = node:addComponent("Camera")
    cam:setClearColor(solo.Vector4(0.0, 0.6, 0.6, 1.0))
    cam:setNear(0.05)

    return cam
end

function initOffscreenCamera()
    local tex = solo.RectTexture.create(dev)
    tex:setData(solo.TextureFormat.RGB, {}, canvasSize.x / 8.0, canvasSize.y / 8.0)
    tex:setFiltering(solo.TextureFiltering.Nearest)
    tex:setWrapping(solo.TextureWrapping.Clamp)

    local node = scene:createNode()
    node:findComponent("Transform"):setLocalPosition(solo.Vector3(0, 0, 10))

    local cam = node:addComponent("Camera")
    cam:setClearColor(solo.Vector4(1, 0, 1, 1))
    cam:setNear(0.05)
    cam:setViewport(solo.Vector4(0, 0, canvasSize.x / 8, canvasSize.y / 8))

    local fb = solo.FrameBuffer.create(dev)
    fb:setAttachments({ tex })
    cam:setRenderTarget(fb)

    return cam, tex
end

function initSkybox()
    local getImagePath = function(fileName) return "../../assets/skyboxes/deep-space/" .. fileName end
    loader:loadCubeTextureAsync({
        getImagePath("front.png"), getImagePath("back.png"), getImagePath("left.png"),
        getImagePath("right.png"), getImagePath("top.png"), getImagePath("bottom.png")
    }):done(function(tex)
        tex:setWrapping(solo.TextureWrapping.Clamp)
        tex:setFiltering(solo.TextureFiltering.Linear)
        local node = scene:createNode()
        local renderer = node:addComponent("SkyboxRenderer")
        renderer:setTexture(tex)
        renderer:setTags(skyboxTag)
    end)
end

function initCheckerBox()
    local effect = solo.Effect.create(dev, shaders.vs.basic, shaders.fs.checker)

    local material = solo.Material.create(dev, effect)
    material:setFaceCull(solo.FaceCull.All)
    material:bindWorldViewProjectionMatrixParameter("worldViewProjMatrix")
    material:setVector4Parameter("color", solo.Vector4(1, 1, 0, 1))

    local node = scene:createNode()
    node:addComponent("MeshRenderer"):setMesh(meshes.cube)
    node:findComponent("MeshRenderer"):setMaterial(0, material)
    node:findComponent("Transform"):setLocalPosition(solo.Vector3(-5, 0, 0))
    -- node:addComponent<Rotator>("world", solo.Vector3(0, 1, 0))
end

function loadTextureAsync(path, callback)
    loader:loadRectTextureAsync(path):done(function(tex)
        tex:generateMipmaps()
        tex:setFiltering(solo.TextureFiltering.LinearMipmapNearest)
        tex:setAnisotropyLevel(8)
        callback(tex)
    end)
end

createDynamicQuadUpdater = dofile("../../src/demos/dynamic-quad-updater.lua")

function createTimeLabelUpdater()
    local textureWidth = 1024
    local textureHeight = 1024
    local lineHeight = 60
    local renderer

    return {
        typeId = 2,

        init = function(self)
            local fontData = fs:readBytes("../../assets/aller.ttf")
            local firstChar = string.byte(" ")
            local charCount = string.byte("~") - string.byte(" ")
            local font = solo.Font.create(dev, fontData, lineHeight, textureWidth,
                textureHeight, firstChar, charCount, 2, 2)

            renderer = self.node:addComponent("FontRenderer")
            renderer:setFont(font)
        end,

        update = function()
            renderer:setText(os.date("Now is %H:%M:%S"))
        end
    }
end

function initDynamicQuad()
    loadTextureAsync("../../assets/freeman.png", function(tex)
        tex:setWrapping(solo.TextureWrapping.Clamp)

        local layout = solo.VertexBufferLayout()
        layout:add(solo.VertexBufferLayoutSemantics.Position, 3)
        layout:add(solo.VertexBufferLayoutSemantics.TexCoord0, 2)

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

        local mesh = solo.Mesh.create(dev)
        mesh:addDynamicVertexBuffer(layout, data, 4)
        mesh:addPart(indices, 6)
        mesh:setPrimitiveType(solo.PrimitiveType.Triangles)

        local material = solo.Material.create(dev, effects.simpleTexture)
        material:setFaceCull(solo.FaceCull.All)
        material:bindWorldViewProjectionMatrixParameter("worldViewProjMatrix")
        material:setTextureParameter("mainTex", tex)

        local node = scene:createNode()
        node:findComponent("Transform"):setLocalPosition(solo.Vector3(0, 0, -5))
        local renderer = node:addComponent("MeshRenderer")
        renderer:setMesh(mesh)
        renderer:setMaterial(0, material)

        node:addScriptComponent(createDynamicQuadUpdater(dev, data, mesh))
    end)
end

function initCurrentTimeLabel()
    local node = scene:createNode()
    node:addScriptComponent(createTimeLabelUpdater())
    node:findComponent("FontRenderer"):setTags(transparentTag)
    node:findComponent("Transform"):setLocalScale(solo.Vector3(0.02, 0.02, 1))
    node:findComponent("Transform"):setLocalPosition(solo.Vector3(-3, 0, 4))
end

function initMonkeyHead(tex)
    local effect = solo.Effect.create(dev, shaders.vs.basicLighting, shaders.fs.textureWithLighting)

    local material = solo.Material.create(dev, effect)
    material:setFaceCull(solo.FaceCull.All)
    material:bindWorldViewProjectionMatrixParameter("worldViewProjMatrix")
    material:bindInvTransposedWorldMatrixParameter("invTransposedWorldMatrix")
    material:setTextureParameter("mainTex", tex)

    loader:loadMeshAsync("../../assets/monkey_hires.obj"):done(function(mesh)
        local node = scene:createNode()
        local renderer = node:addComponent("MeshRenderer")
        renderer:setMesh(mesh)
        renderer:setMaterial(0, material)
        node:findComponent("Transform"):setLocalPosition(solo.Vector3(0, 0, 0))
        -- node:addComponent<Rotator>("local", Vector3::unitX())
    end)
end

function initFloor(tex)
    local material = solo.Material.create(dev, effects.simpleTexture)
    material:setFaceCull(solo.FaceCull.All)
    material:bindWorldViewProjectionMatrixParameter("worldViewProjMatrix")
    material:setTextureParameter("mainTex", tex)

    local node = scene:createNode()
    
    node:findComponent("Transform"):setLocalScale(solo.Vector3(10, 0.1, 10))
    node:findComponent("Transform"):setLocalPosition(solo.Vector3(0, -2, 0))

    local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(meshes.cube)
    renderer:setMaterial(0, material)

    local params = solo.RigidBodyConstructionParameters()
    params.mass = 0
    params.friction = 0.5
    local rigidBody = node:addComponent("RigidBody", params)
    rigidBody:setCollider(solo.BoxCollider.create(solo.Vector3(1, 1, 1)))
end

mainCamera = initMainCamera()
offscreenCamera, offscreenCameraTex = initOffscreenCamera()
initSkybox()
initCheckerBox()
initDynamicQuad()
initCurrentTimeLabel()
loadTextureAsync("../../assets/cobblestone.png", function(tex)
    initMonkeyHead(tex)
    initFloor(tex)
end)

-- TODO monitor quad, axes, etc...

function keepRunning()
    return not dev:isQuitRequested() and
           not dev:isWindowCloseRequested() and
           not dev:isKeyPressed(solo.KeyCode.Escape, true)
end

function update()
    scene:visit(function(cmp)
        cmp:update()
    end)
end

function renderByTags(tags, ctx)
    scene:visitByTags(tags, function(cmp) cmp:render(ctx) end)
end

function render()
    mainCamera:renderFrame(function(ctx)
        renderByTags(skyboxTag, ctx)
        renderByTags(~(skyboxTag | transparentTag), ctx)
        renderByTags(transparentTag, ctx)
    end)
end

function run()
    while keepRunning() do
        dev:update(function()
            loader:update()
                loader:update()
                physics:update()
                renderer:renderFrame(function()
                    update()
                    render()
                end)
        end)
    end
end

function runSafe()
    local _, err = select(1, pcall(run))
    if err then
        logger:logError(err)
    end
end

runSafe()