--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

package.path = "../../src/demos/lua/?.lua;" .. package.path

require "common"

function demo()
    local dev = sl.device
    local physics = dev:getPhysics()
    local fs = dev:getFileSystem()
    local scene = sl.Scene.create(dev)

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

    ---

    function createLightCamera()
        local node = scene:createNode()

        local cam = node:addComponent("Camera")
        cam:setZNear(0.05)
        cam:setZFar(1000)
        cam:setFOV(sl.Radians.fromRawDegrees(50))
        cam:setViewport(vec4(0, 0, 2048, 2048))

        local depthTex = sl.Texture2D.createEmpty(sl.device, 2048, 2048, sl.TextureFormat.Depth24)
        depthTex:setFilter(sl.TextureFilter.Nearest, sl.TextureFilter.Nearest, sl.TextureMipFilter.Nearest)

        local fb = sl.FrameBuffer.create(dev, { depthTex })
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
        layout:addSemanticAttribute(sl.VertexAttributeSemantics.Position)
        layout:addSemanticAttribute(sl.VertexAttributeSemantics.Normal)
        layout:addSemanticAttribute(sl.VertexAttributeSemantics.TexCoord)
        sl.Mesh.loadFromFileAsync(dev, getAssetPath(meshPath), layout)
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

    ---

    local lightCam = createLightCamera()

    local colorPassEffect = assetCache.getEffect("shadowed")
    local colorPassMaterial = sl.Material.create(dev, colorPassEffect)
    colorPassMaterial:setFaceCull(sl.FaceCull.None)
    colorPassMaterial:bindParameter("uniforms:wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)
    colorPassMaterial:bindParameter("uniforms:model", sl.BindParameterSemantics.WorldMatrix)
    colorPassMaterial:setTextureParameter("mainTex", assetCache.textures.cobbleStone)
    colorPassMaterial:setTextureParameter("shadowMap", lightCam.depthTex)

    local shadowDepthEffect = assetCache.getEffect("shadow-depth")
    local depthPassMaterial = sl.Material.create(dev, shadowDepthEffect)
    depthPassMaterial:setFaceCull(sl.FaceCull.None)
    depthPassMaterial:bindParameter("matrices:wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)

    local mainCamera, mainCameraNode = createMainCamera(scene)
    mainCameraNode:findComponent("Transform"):setLocalPosition(vec3(10, 10, -5))
    mainCameraNode:findComponent("Transform"):lookAt(vec3(0, 2, 0), vec3(0, 1, 0))
    mainCameraNode:addScriptComponent(createTracer(physics))
    mainCameraNode:addScriptComponent(createSpawner(assetCache))

    local postProcessor = createPostProcessor(assetCache, mainCamera)
    local postProcessorControlPanel = createPostProcessorControlPanel(assetCache, mainCameraNode, postProcessor)
    postProcessorControlPanel.transform:setLocalPosition(vec3(-7, 0, -5))
    postProcessorControlPanel.transform:rotateByAxisAngle(vec3(0, 1, 0), sl.Radians.fromRawDegrees(90), sl.TransformSpace.World)
    mainCameraNode:addScriptComponent(postProcessorControlPanel.cmp)

    local skybox = createSkybox(scene, assetCache)
    local backdrop = createMesh("meshes/backdrop.obj", colorPassMaterial)
    local dynamicQuad = createDynamicQuad(scene, assetCache)
    dynamicQuad.transform:setLocalPosition(vec3(3, 1, 3))

    local teapot = createMesh("meshes/teapot.obj", colorPassMaterial)
    teapot.transform:setLocalPosition(vec3(3, 0, -3))
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
        return dev:isQuitRequested() or
               dev:isWindowCloseRequested() or
               dev:isKeyPressed(sl.KeyCode.Escape, true)
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
        colorPassMaterial:setMatrixParameter("uniforms:lightVp", lightCam.camera:getViewProjectionMatrix())
        colorPassMaterial:setVector3Parameter("uniforms:lightPos", lightCam.transform:getWorldPosition())
        lightCam.camera:renderFrame(renderLightCamFrame)
        mainCamera:renderFrame(renderMainCamFrame)
        postProcessor:apply()
    end

    function run()
        while not shouldStop() do
            dev:update(update)
            collectgarbage("collect")
        end
    end

    run()
end

callSafe(demo)