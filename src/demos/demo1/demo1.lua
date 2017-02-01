dofile("../../src/demos/common/common.lua")

collectgarbage("setpause", 100)

createScreenshoter = dofile("../../src/demos/common/screenshoter.lua")
createRotator = dofile("../../src/demos/common/rotator.lua")
createLookAt = dofile("../../src/demos/common/lookat.lua")
createDynamicQuadUpdater = dofile("../../src/demos/demo1/dynamic-quad-updater.lua")
createTimeLabelUpdater = dofile("../../src/demos/demo1/time-label-updater.lua")
createSpawner = dofile("../../src/demos/demo1/spawner.lua")
createSpawnedObjectTargeter = dofile("../../src/demos/demo1/spawned-object-targeter.lua")
postProcessors = dofile("../../src/demos/demo1/post-processors.lua")
shaders = dofile("../../src/demos/demo1/shaders.lua")

dev = solo.device
loader = dev:getAssetLoader()
physics = dev:getPhysics()
renderer = dev:getRenderer()
logger = dev:getLogger()
fs = dev:getFileSystem()
scene = solo.Scene.create(dev)
canvasSize = dev:getCanvasSize()

function getAssetPath(fileName)
    return "../../assets/" .. fileName
end

knownTags = {
    skybox = 1 << 1,
    transparent = 1 << 2,
    monitor = 1 << 3,
    postProcessor = 1 << 4
}

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
    red = createColorMaterial(vec4(1, 0, 0, 1)),
    green = createColorMaterial(vec4(0, 1, 0, 1)),
    blue = createColorMaterial(vec4(0, 0, 1, 1)),
    white = createColorMaterial(vec4(1, 1, 1, 1))
}

meshes = {
    cube = solo.Mesh.createFromPrefab(dev, solo.MeshPrefab.Cube),
    quad = solo.Mesh.createFromPrefab(dev, solo.MeshPrefab.Quad)
}

function initMainCamera()
    local node = scene:createNode()
        
    local t = node:findComponent("Transform")
    t:setLocalPosition(vec3(0, 5, 10))
    t:lookAt(vec3(0, 0, 0), vec3(0, 1, 0))
    
    node:addScriptComponent(createScreenshoter(dev, "demo1.bmp"))
    node:addComponent("Spectator")
    node:addScriptComponent(createSpawnedObjectTargeter(physics))
    node:addScriptComponent(createSpawner(dev, meshes.cube, effects.color))

    local cam = node:addComponent("Camera")
    cam:setClearColor(vec4(0.0, 0.6, 0.6, 1.0))
    cam:setNear(0.05)

    return cam
end

function initOffscreenCamera()
    local tex = solo.RectTexture.create(dev)
    tex:setData(solo.TextureFormat.RGB, {}, math.floor(canvasSize.x / 8.0), math.floor(canvasSize.y / 8.0))
    tex:setFiltering(solo.TextureFiltering.Nearest)
    tex:setWrapping(solo.TextureWrapping.Clamp)

    local node = scene:createNode()
    node:findComponent("Transform"):setLocalPosition(vec3(0, 0, 10))

    local cam = node:addComponent("Camera")
    cam:setClearColor(vec4(1, 0, 1, 1))
    cam:setNear(0.05)
    cam:setViewport(vec4(0, 0, canvasSize.x / 8, canvasSize.y / 8))

    local fb = solo.FrameBuffer.create(dev)
    fb:setAttachments({ tex })
    cam:setRenderTarget(fb)

    return cam, tex
end

function initSkybox()
    local getImagePath = function(fileName) return getAssetPath("skyboxes/deep-space/") .. fileName end
    loader:loadCubeTextureAsync({
        getImagePath("front.png"), getImagePath("back.png"), getImagePath("left.png"),
        getImagePath("right.png"), getImagePath("top.png"), getImagePath("bottom.png")
    }):done(function(tex)
        tex:setWrapping(solo.TextureWrapping.Clamp)
        tex:setFiltering(solo.TextureFiltering.Linear)
        local node = scene:createNode()
        local renderer = node:addComponent("SkyboxRenderer")
        renderer:setTexture(tex)
        renderer:setTag(knownTags.skybox)
    end)
end

function initCheckerBox()
    local effect = solo.Effect.create(dev, shaders.vs.basic, shaders.fs.checker)

    local material = solo.Material.create(dev, effect)
    material:setFaceCull(solo.FaceCull.All)
    material:bindWorldViewProjectionMatrixParameter("worldViewProjMatrix")
    material:setVector4Parameter("color", vec4(1, 1, 0, 1))

    local node = scene:createNode()
    node:addComponent("MeshRenderer"):setMesh(meshes.cube)
    node:findComponent("MeshRenderer"):setMaterial(0, material)
    node:findComponent("Transform"):setLocalPosition(vec3(-5, 0, 0))
    node:addScriptComponent(createRotator(dev, "world", vec3(0, 1, 0)))
end

function loadTextureAsync(path, callback)
    loader:loadRectTextureAsync(path):done(function(tex)
        tex:generateMipmaps()
        tex:setFiltering(solo.TextureFiltering.LinearMipmapNearest)
        tex:setAnisotropyLevel(8)
        callback(tex)
    end)
end

function initDynamicQuad()
    loadTextureAsync(getAssetPath("freeman.png"), function(tex)
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
        node:findComponent("Transform"):setLocalPosition(vec3(0, 0, -5))
        local renderer = node:addComponent("MeshRenderer")
        renderer:setMesh(mesh)
        renderer:setMaterial(0, material)

        node:addScriptComponent(createDynamicQuadUpdater(dev, data, mesh))
    end)
end

function initCurrentTimeLabel()
    local node = scene:createNode()
    node:addScriptComponent(createTimeLabelUpdater(dev, fs))
    node:findComponent("FontRenderer"):setTag(knownTags.transparent)
    node:findComponent("Transform"):setLocalScale(vec3(0.02, 0.02, 1))
    node:findComponent("Transform"):setLocalPosition(vec3(-3, 0, 4))
end

function initMonkeyHead(tex)
    local effect = solo.Effect.create(dev, shaders.vs.basicLighting, shaders.fs.textureWithLighting)

    local material = solo.Material.create(dev, effect)
    material:setFaceCull(solo.FaceCull.All)
    material:bindWorldViewProjectionMatrixParameter("worldViewProjMatrix")
    material:bindInvTransposedWorldMatrixParameter("invTransposedWorldMatrix")
    material:setTextureParameter("mainTex", tex)

    loader:loadMeshAsync(getAssetPath("monkey_hires.obj")):done(function(mesh)
        local node = scene:createNode()
        local renderer = node:addComponent("MeshRenderer")
        renderer:setMesh(mesh)
        renderer:setMaterial(0, material)
        node:findComponent("Transform"):setLocalPosition(vec3(0, 0, 0))
        node:addScriptComponent(createRotator(dev, "local", vec3(1, 0, 0)))
    end)
end

function initFloor(tex)
    local material = solo.Material.create(dev, effects.simpleTexture)
    material:setFaceCull(solo.FaceCull.All)
    material:bindWorldViewProjectionMatrixParameter("worldViewProjMatrix")
    material:setTextureParameter("mainTex", tex)

    local node = scene:createNode()
    
    node:findComponent("Transform"):setLocalScale(vec3(10, 0.1, 10))
    node:findComponent("Transform"):setLocalPosition(vec3(0, -2, 0))

    local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(meshes.cube)
    renderer:setMaterial(0, material)

    local params = solo.RigidBodyConstructionParameters()
    params.mass = 0
    params.friction = 0.5
    local rigidBody = node:addComponent("RigidBody", params)
    rigidBody:setCollider(solo.BoxCollider.create(vec3(1, 1, 1)))
end

function attachAxesMesh(node, axesMesh)
    local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(axesMesh)
    renderer:setMaterial(0, materials.blue)
    renderer:setMaterial(1, materials.green)
    renderer:setMaterial(2, materials.white)
    renderer:setMaterial(3, materials.red)
end

function initMonitorQuad(axesMesh)
    local material = solo.Material.create(dev, effects.simpleTexture)
    material:setFaceCull(solo.FaceCull.All)
    material:bindWorldViewProjectionMatrixParameter("worldViewProjMatrix")
    material:setTextureParameter("mainTex", offscreenCameraTex)

    local parent = scene:createNode()
    parent:findComponent("Transform"):setLocalPosition(vec3(-2, 2, -2))
    parent:addScriptComponent(createRotator(dev, "world", vec3(0, 1, 0)))
    attachAxesMesh(parent, axesMesh)

    local node = scene:createNode()
    node:addScriptComponent(createLookAt(vec3(0, 0, 0)))

    local transform = node:findComponent("Transform")
    transform:setParent(parent:findComponent("Transform"))
    transform:setLocalPosition(vec3(5, 2, -5))
    transform:setLocalScale(vec3(5, 5 * canvasSize.y / canvasSize.x, 1))
    
    local renderer = node:addComponent("MeshRenderer")
    renderer:setMesh(meshes.quad)
    renderer:setMaterial(0, material)
    renderer:setTag(knownTags.monitor)
end

function initTransparentQuad(axesMesh)
    loadTextureAsync(getAssetPath("flammable.png"), function(tex)
        tex:setWrapping(solo.TextureWrapping.Clamp)

        local material = solo.Material.create(dev, effects.simpleTexture)
        material:setFaceCull(solo.FaceCull.All)
        material:bindWorldViewProjectionMatrixParameter("worldViewProjMatrix")
        material:setTextureParameter("mainTex", tex)
        material:setBlend(true)
        material:setDepthTest(true)
        material:setDepthWrite(false)

        local parent = scene:createNode()
        parent:findComponent("Transform"):setLocalPosition(vec3(5, 0, 0))
        parent:addScriptComponent(createRotator(dev, "world", vec3(0, 1, 0)))
        attachAxesMesh(parent, axesMesh)

        local node = scene:createNode()
        node:addScriptComponent(createRotator(dev, "local", vec3(1, 0, 0)))
        node:findComponent("Transform"):setParent(parent:findComponent("Transform"))
        node:findComponent("Transform"):setLocalPosition(vec3(2, 0, 0))

        local renderer = node:addComponent("MeshRenderer")
        renderer:setMesh(meshes.quad)
        renderer:setMaterial(0, material)
        renderer:setTag(knownTags.transparent)
    end)
end

mainCamera = initMainCamera()
offscreenCamera, offscreenCameraTex = initOffscreenCamera()
initSkybox()
initCheckerBox()
initDynamicQuad()
initCurrentTimeLabel()
loadTextureAsync(getAssetPath("cobblestone.png"), function(tex)
    initMonkeyHead(tex)
    initFloor(tex)
end)

loader:loadMeshAsync(getAssetPath("axes.obj")):done(function(mesh)
    initMonitorQuad(mesh)
    initTransparentQuad(mesh)
end)

function keepRunning()
    return not dev:isQuitRequested() and
           not dev:isWindowCloseRequested() and
           not dev:isKeyPressed(solo.KeyCode.Escape, true)
end

function detachPostProcessor()
    if pp then
        pp:detach()
        pp = nil
    end
end

function update()
    scene:visit(function(cmp) cmp:update() end)

    if dev:isKeyPressed(solo.KeyCode.Digit1, true) then
        detachPostProcessor()
        pp = postProcessors.create1(dev, mainCamera, knownTags.postProcessor, shaders)
    end

    if dev:isKeyPressed(solo.KeyCode.Digit2, true) then
        detachPostProcessor()
        pp = postProcessors.create2(dev, loader, mainCamera, knownTags.postProcessor, shaders)
    end

    if dev:isKeyPressed(solo.KeyCode.Digit3, true) then
        detachPostProcessor()
    end

    if dev:isKeyPressed(solo.KeyCode.X, true) then
        print(getCmpId("test"))
        print(getCmpId("test2"))
    end
end

function renderByTags(tags, ctx)
    scene:visitByTags(tags, function(cmp) cmp:render(ctx) end)
end

function render()
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
