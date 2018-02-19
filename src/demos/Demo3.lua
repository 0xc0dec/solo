--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

package.path = "../../src/demos/?.lua;" .. package.path

require "Common"

function demo()
    local dev = sl.device
    local physics = dev:getPhysics()
    local renderer = dev:getRenderer()
    local fs = dev:getFileSystem()
    local scene = sl.Scene.create(dev)

    local tags = require "Tags"
    local createRotator = require "Rotator"
    local createMainCamera = require "MainCamera"
    local createSkybox = require "Skybox"
    local assetCache = (require "AssetCache")()

    --

    local effectDesc = {
        vertex = {
            uniformBuffers = {
                matrices = {
                    wvp = "mat4",
                    model = "mat4",
                    lightVp = "mat4"
                }
            },
    
            inputs = {
                slPosition = "vec3",
                slTexCoord = "vec2"
            },
    
            outputs = {
                uv = "vec2",
                shadowCoord = "vec4"
            },
    
            entry = [[
                const mat4 biasMat = mat4( 
                    0.5, 0.0, 0.0, 0.0,
                    0.0, 0.5, 0.0, 0.0,
                    0.0, 0.0, 0.5, 0.0,
                    0.5, 0.5, 0.5, 1.0);

                uv = slTexCoord;
                gl_Position = #matrices:wvp# * vec4(slPosition, 1);
                shadowCoord = (biasMat * #matrices:lightVp# * #matrices:model#) * vec4(slPosition, 1.0);
                
                FIX_UV#uv#;
                FIX_Y#gl_Position#;
            ]]
        },
    
        fragment = {
            samplers = {
                mainTex = "sampler2D",
                shadowMap = "sampler2D"
            },
    
            outputs = {
                fragColor = "vec4"
            },
    
            entry = [[
                float shadow = 1.0;
                vec4 x = shadowCoord / shadowCoord.w;
                float dist = texture(shadowMap, x.st).r;
                if (dist < x.z - 0.00006)
                    shadow = 0.1;

                fragColor = texture(mainTex, uv) * shadow;
            ]]
        }
    }

    ---

    function createFloor(material)
        local node = scene:createNode()
    
        local transform = node:findComponent("Transform")
        transform:setLocalScale(vec3(10, 0.1, 10))
        transform:setLocalPosition(vec3(0, -2, 0))
    
        local renderer = node:addComponent("MeshRenderer")
        renderer:setMesh(assetCache.meshes.cube)
        renderer:setMaterial(0, material)
    end

    function createLightCamera()
        local node = scene:createNode()

        local cam = node:addComponent("Camera")
        -- cam:setPerspective(false)
        -- cam:setOrthoSize(vec2(30, 30))
        cam:setZNear(0.05)
        cam:setZFar(10000)
        cam:setViewport(vec4(0, 0, 1024, 1024))

        local depthTex = sl.Texture2D.createEmpty(sl.device, 1024, 1024, sl.TextureFormat.Depth)
        depthTex:setFilter(sl.TextureFilter.Nearest, sl.TextureFilter.Nearest, sl.TextureMipFilter.Nearest)

        local fb = sl.FrameBuffer.create(dev, { depthTex })
        cam:setRenderTarget(fb)
    
        local transform = node:findComponent("Transform")
        transform:setLocalPosition(vec3(-10, 10, -10))
        transform:lookAt(vec3(0, 0, 0), vec3(0, 1, 0))

        local rootNode = scene:createNode()
        rootNode:addScriptComponent(createRotator("world", vec3(0, 1, 0), 2))
        transform:setParent(rootNode:findComponent("Transform"))

        return cam, node, transform, depthTex
    end

    function createModel(material)
        local node = scene:createNode()
    
        local renderer = node:addComponent("MeshRenderer")
        renderer:setMaterial(0, material)
    
        local transform = node:findComponent("Transform")
        transform:setLocalRotation(sl.Quaternion.createFromAxisAngle(vec3(0, 0, 1), sl.Radians.fromRawDegrees(180)))
        transform:setLocalPosition(vec3(0, -1, 0))
    
        sl.Mesh.loadFromFileAsync(dev, getAssetPath("meshes/Teapot.obj")):done(
            function(mesh)
                renderer:setMesh(mesh)
            end)
    end

    ---

    local lightCam, lightCamNode, lightCamTransform, depthTex = createLightCamera()

    local effectSrc = sl.generateEffectSource(effectDesc)
    local effect = sl.Effect.createFromSource(dev, effectSrc)
    local material = sl.Material.create(dev, effect)
    material:setFaceCull(sl.FaceCull.None)
    material:bindParameter("matrices:wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)
    material:bindParameter("matrices:model", sl.BindParameterSemantics.WorldMatrix)
    material:setTextureParameter("mainTex", assetCache.textures.cobbleStone)
    material:setTextureParameter("shadowMap", depthTex)

    lightCamNode:addScriptComponent(sl.createComponent("MaterialUpdater", {
        update = function()
            material:setMatrixParameter("matrices:lightVp", lightCam:getViewProjectionMatrix())
        end
    }))

    local mainCam, camNode = createMainCamera(scene)
    camNode:findComponent("Transform"):setLocalPosition(vec3(5, 5, 5))
    camNode:findComponent("Transform"):lookAt(vec3(0, 0, 0), vec3(0, 1, 0))

    createSkybox(scene)
    createFloor(material)
    createModel(material)

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
        scene:visitByTags(~tags.skybox, renderCmp)
    end

    function renderMainCamFrame()
        scene:visitByTags(tags.skybox, renderCmp)
        scene:visitByTags(~tags.skybox, renderCmp)
    end

    function update()
        scene:visit(updateCmp)
        lightCam:renderFrame(renderLightCamFrame)
        mainCam:renderFrame(renderMainCamFrame)
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
