--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

sl.includeDir("../../src/demos/lua/demo1")
sl.includeDir("../../src/demos/lua/common")

require "common"

function demo()
    local fs = sl.device:fileSystem()
    local physics = sl.device:physics()
    local scene = sl.Scene.empty(sl.device)

    local tags = require "tags"
    local createRotator = require "rotator"
    local createMainCamera = require "main-camera"
    local createSkybox = require "skybox"
    local assetCache = (require "asset-cache")()
    local createPostProcessor = require "post-processor"
    local createPostProcessorControlPanel = require "post-processor-control-panel"
    local createTracer = require "tracer"
    local createSpawner = require "spawner"
    local createCheckerBox = require "checker-box"
    local createDynamicQuad = require "dynamic-quad"
    local attachAxes = (require "axes")(assetCache)
    local createLookAt = require "look-at"
    local createHighlighter = require "highlighter"

    ---

    function createLightCamera()
        local node = scene:createNode()

        local cam = node:addComponent("Camera")
        cam:setZNear(0.05)
        cam:setZFar(1000)
        cam:setFieldOfView(sl.Radians.fromRawDegrees(50))
        cam:setViewport(vec4(0, 0, 2048, 2048))

        local depthTex = sl.Texture2D.empty(sl.device, 2048, 2048, sl.TextureFormat.Depth24)
        depthTex:setFilter(sl.TextureFilter.Nearest, sl.TextureFilter.Nearest, sl.TextureMipFilter.Nearest)

        local fb = sl.FrameBuffer.create(sl.device, { depthTex })
        cam:setRenderTarget(fb)
    
        local transform = node:findComponent("Transform")
        transform:setLocalPosition(vec3(-20, 20, -20))
        transform:lookAt(vec3(0, 0, 0), vec3(0, 1, 0))

        local rootNode = scene:createNode()
        rootNode:addScriptComponent(createRotator("world", vec3(0, 1, 0), 0.3))
        transform:setParent(rootNode:findComponent("Transform"))

        return {
            camera = cam,
            node = node,
            transform = transform,
            depthTex = depthTex
        }
    end

    function createMesh(meshPath, material)
        local node = scene:createNode()
    
        local renderer = node:addComponent("MeshRenderer")
        renderer:setMaterial(0, material)
    
        local transform = node:findComponent("Transform")
    
        local layout = sl.VertexBufferLayout()
        layout:addAttribute(sl.VertexAttributeUsage.Position)
        layout:addAttribute(sl.VertexAttributeUsage.Normal)
        layout:addAttribute(sl.VertexAttributeUsage.TexCoord)
        sl.Mesh.fromFileAsync(sl.device, assetPath(meshPath), layout)
            :done(function(mesh) renderer:setMesh(mesh) end)

        return {
            node = node,
            transform = transform,
            renderer = renderer
        }
    end

    function createTracker(parentTransform, pos, target)
        local node = scene:createNode()
        local transform = node:findComponent("Transform")
        transform:setParent(parentTransform)
        transform:setLocalPosition(pos)
        node:addScriptComponent(createLookAt(target))
        attachAxes(node)
    end

    function createBackdrop(material)
        local backdrop = createMesh("meshes/backdrop.obj", material)
        
        local params = sl.RigidBodyParams()
        params.mass = 0
        params.friction = 0.5
        local body = backdrop.node:addComponent("RigidBody", params)
        body:setKinematic(true)
        
        sl.StaticMeshCollider.fromFileAsync(sl.device, assetPath("meshes/backdrop.obj"))
            :done(function(col) body:setCollider(col) end)

        return backdrop
    end

    function createTeapot(mat)
        local teapot = createMesh("meshes/teapot.obj", mat)
        teapot.transform:setLocalPosition(vec3(3, 0, -3))

        local params = sl.RigidBodyParams()
        params.mass = 0
        params.friction = 0.5
        local body = teapot.node:addComponent("RigidBody", params)
        body:setKinematic(true)

        sl.StaticMeshCollider.fromFileAsync(sl.device, assetPath("meshes/teapot.obj"))
            :done(function(col) body:setCollider(col) end)

        return teapot
    end

    ---

    local lightCam = createLightCamera()

    local colorPassEffect = assetCache.getEffect("shadowed")
    local colorPassMaterial = sl.Material.fromEffect(sl.device, colorPassEffect)
    colorPassMaterial:setFaceCull(sl.FaceCull.None)
    colorPassMaterial:bindParameter("uniforms:wvp", sl.ParameterBinding.WorldViewProjectionMatrix)
    colorPassMaterial:bindParameter("uniforms:model", sl.ParameterBinding.WorldMatrix)
    colorPassMaterial:setTextureParameter("mainTex", assetCache.textures.cobbleStone)
    colorPassMaterial:setTextureParameter("shadowMap", lightCam.depthTex)

    local shadowDepthEffect = assetCache.getEffect("shadow-depth")
    local depthPassMaterial = sl.Material.fromEffect(sl.device, shadowDepthEffect)
    depthPassMaterial:setFaceCull(sl.FaceCull.None)
    depthPassMaterial:bindParameter("matrices:wvp", sl.ParameterBinding.WorldViewProjectionMatrix)

    local mainCamera, mainCameraNode = createMainCamera(scene)
    mainCameraNode:findComponent("Transform"):setLocalPosition(vec3(10, 10, -5))
    mainCameraNode:findComponent("Transform"):lookAt(vec3(0, 2, 0), vec3(0, 1, 0))
    mainCameraNode:addScriptComponent(createTracer(sl.device, scene, physics, assetCache))
    mainCameraNode:addScriptComponent(createSpawner(assetCache, colorPassMaterial))
    mainCameraNode:addScriptComponent(createHighlighter(assetCache, physics))

    local postProcessor = createPostProcessor(assetCache, mainCamera)
    local ppControlPanel = createPostProcessorControlPanel(assetCache, mainCameraNode, postProcessor)
    ppControlPanel.transform:setLocalPosition(vec3(-7, 0, -5))
    ppControlPanel.transform:rotateByAxisAngle(vec3(0, 1, 0),
        sl.Radians.fromRawDegrees(90), sl.TransformSpace.World)
    mainCameraNode:addScriptComponent(ppControlPanel.cmp)

    local skybox = createSkybox(scene, assetCache)
    local backdrop = createBackdrop(colorPassMaterial)
    local dynamicQuad = createDynamicQuad(scene, assetCache)
    dynamicQuad.transform:setLocalPosition(vec3(3, 1, 3))

    local teapot = createTeapot(colorPassMaterial)

    local checkerBox = createCheckerBox(scene, assetCache)
    checkerBox.transform:setLocalPosition(vec3(-3, 1, 3))

    local rootNode = scene:createNode()
    local rootNodeTransform = rootNode:findComponent("Transform")
    attachAxes(rootNode)
    rootNode:addScriptComponent(createRotator("world", vec3(0, 1, 0), 1))

    createTracker(rootNodeTransform, vec3(4, 4, 4), vec3(3, 0, -3))
    createTracker(rootNodeTransform, vec3(-4, 4, 4), vec3(-3, 1, 3))
    createTracker(rootNodeTransform, vec3(4, 4, -4), vec3(3, 1, 3))

    ---

    function shouldStop()
        return sl.device:isQuitRequested() or
               sl.device:isWindowCloseRequested() or
               sl.device:isKeyPressed(sl.KeyCode.Escape, true)
    end

    function renderCmp(cmp)
        cmp:render()
    end

    function updateCmp(cmp)
        cmp:update()
    end

    function renderLightCamFrame()
        backdrop.renderer:setMaterial(0, depthPassMaterial)
        scene:visitByTags(~(skybox.tag | tags.postProcessorStep), renderCmp)
    end

    function renderMainCamFrame()
        backdrop.renderer:setMaterial(0, colorPassMaterial)
        scene:visitByTags(skybox.tag, renderCmp)
        scene:visitByTags(~(skybox.tag | tags.postProcessorStep | tags.transparent), renderCmp)
        scene:visitByTags(tags.transparent, renderCmp)
    end

    function update()
        scene:visit(updateCmp)
        colorPassMaterial:setMatrixParameter("uniforms:lightVp", lightCam.camera:viewProjectionMatrix())
        colorPassMaterial:setVector3Parameter("uniforms:lightPos", lightCam.transform:worldPosition())
        lightCam.camera:renderFrame(renderLightCamFrame)
        mainCamera:renderFrame(renderMainCamFrame)
        postProcessor:apply()
    end

    function run()
        while not shouldStop() do
            sl.device:update(update)
            collectgarbage("collect")
        end
    end

    run()
end

callSafe(demo)
