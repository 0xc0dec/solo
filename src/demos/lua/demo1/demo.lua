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
    local createLightCamera = require "light-camera"

    ---

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

    function createShadowedMaterial(depthTex)
        local eff = assetCache.getEffect("shadowed")
        local mat = sl.Material.fromEffect(sl.device, eff)
        mat:setFaceCull(sl.FaceCull.None)
        mat:bindParameter("uniforms:wvp", sl.ParameterBinding.WorldViewProjectionMatrix)
        mat:bindParameter("uniforms:model", sl.ParameterBinding.WorldMatrix)
        mat:setTextureParameter("mainTex", assetCache.textures.cobbleStone)
        mat:setTextureParameter("shadowMap", depthTex)
        return mat
    end

    function createSpectatorCamera(shadowedMat)
        local camera, node = createMainCamera(scene)
        node:findComponent("Transform"):setLocalPosition(vec3(10, 10, -5))
        node:findComponent("Transform"):lookAt(vec3(0, 2, 0), vec3(0, 1, 0))
        node:addScriptComponent(createTracer(sl.device, scene, physics, assetCache))
        node:addScriptComponent(createSpawner(assetCache, shadowedMat))
        node:addScriptComponent(createHighlighter(assetCache, physics))

        return {
            camera = camera,
            node = node
        }
    end

    function createAxes()
        local rootNode = scene:createNode()
        local rootTransform = rootNode:findComponent("Transform")
        attachAxes(rootNode)
        rootNode:addScriptComponent(createRotator("world", vec3(0, 1, 0), 1))

        createTracker(rootTransform, vec3(4, 4, 4), vec3(3, 0, -3))
        createTracker(rootTransform, vec3(-4, 4, 4), vec3(-3, 1, 3))
        createTracker(rootTransform, vec3(4, 4, -4), vec3(3, 1, 3))
    end

    ---

    local lightCam = createLightCamera(scene)
    local shadowedMat = createShadowedMaterial(lightCam.depthTex)
    local mainCamera = createSpectatorCamera(shadowedMat)

    local postProcessor = createPostProcessor(assetCache, mainCamera.camera)
    local ppControlPanel = createPostProcessorControlPanel(assetCache, mainCamera.node, postProcessor)
    ppControlPanel.transform:setLocalPosition(vec3(-7, 0, -5))
    ppControlPanel.transform:rotateByAxisAngle(vec3(0, 1, 0),
        sl.Radians.fromRawDegrees(90), sl.TransformSpace.World)

    mainCamera.node:addScriptComponent(ppControlPanel.cmp)

    local skybox = createSkybox(scene, assetCache)
    local backdrop = createBackdrop(shadowedMat)
    
    local dynamicQuad = createDynamicQuad(scene, assetCache)
    dynamicQuad.transform:setLocalPosition(vec3(3, 1, 3))

    local teapot = createTeapot(shadowedMat)

    local checkerBox = createCheckerBox(scene, assetCache)
    checkerBox.transform:setLocalPosition(vec3(-3, 1, 3))

    createAxes()

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
        scene:visitByTags(~(skybox.tag | tags.postProcessorStep), renderCmp)
    end

    function renderMainCamFrame()
        scene:visitByTags(skybox.tag, renderCmp)
        scene:visitByTags(~(skybox.tag | tags.postProcessorStep | tags.transparent), renderCmp)
        scene:visitByTags(tags.transparent, renderCmp)
    end

    function update()
        scene:visit(updateCmp)
        shadowedMat:setMatrixParameter("uniforms:lightVp", lightCam.camera:viewProjectionMatrix())
        shadowedMat:setVector3Parameter("uniforms:lightPos", lightCam.transform:worldPosition())
        lightCam.camera:renderFrame(renderLightCamFrame)
        mainCamera.camera:renderFrame(renderMainCamFrame)
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
