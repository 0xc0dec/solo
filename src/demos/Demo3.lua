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

    local depthPassEffectDesc = {
        vertex = {
            uniformBuffers = {
                matrices = {
                    wvp = "mat4"
                }
            },
    
            inputs = {
                sl_Position = "vec3"
            },
    
            outputs = {
            },
    
            entry = [[
                gl_Position = #matrices:wvp# * vec4(sl_Position, 1);
                SL_FIX_Y#gl_Position#;
            ]]
        },
    
        fragment = {
            samplers = {
            },
    
            outputs = {
                fragColor = "vec4"
            },
    
            entry = [[
                fragColor = vec4(1, 0, 0, 1);
            ]]
        }
    }

    local colorPassEffectDesc = {
        vertex = {
            uniformBuffers = {
                matrices = {
                    wvp = "mat4",
                    model = "mat4",
                    lightVp = "mat4"
                }
            },
    
            inputs = {
                sl_Position = "vec3",
                slTexCoord = "vec2"
            },
    
            outputs = {
                uv = "vec2",
                shadowCoord = "vec4"
            },
    
            entry = [[
                const mat4 biasMat = SL_SHADOW_BIAS_MAT;

                uv = slTexCoord;
                SL_FIX_UV#uv#;

                gl_Position = #matrices:wvp# * vec4(sl_Position, 1);
                SL_FIX_Y#gl_Position#;

                vec4 lightProjectedPos = (#matrices:lightVp# * #matrices:model#) * vec4(sl_Position, 1.0);
                SL_FIX_Y#lightProjectedPos#;
                shadowCoord = biasMat * lightProjectedPos;
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
                if (dist < x.z - 0.00001)
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

        return renderer
    end

    function createLightCamera()
        local node = scene:createNode()

        local cam = node:addComponent("Camera")
        cam:setZNear(0.05)
        cam:setZFar(1000)
        cam:setViewport(vec4(0, 0, 1024, 1024))

        local depthTex = sl.Texture2D.createEmpty(sl.device, 1024, 1024, sl.TextureFormat.Depth)
        depthTex:setFilter(sl.TextureFilter.Nearest, sl.TextureFilter.Nearest, sl.TextureMipFilter.Nearest)

        local fb = sl.FrameBuffer.create(dev, { depthTex })
        cam:setRenderTarget(fb)
    
        local transform = node:findComponent("Transform")
        transform:setLocalPosition(vec3(-20, 20, -20))
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

        return renderer
    end

    ---

    local lightCam, lightCamNode, lightCamTransform, depthTex = createLightCamera()

    local colorPassEffect = sl.Effect.createFromSource(dev, sl.generateEffectSource(colorPassEffectDesc))
    local colorPassMaterial = sl.Material.create(dev, colorPassEffect)
    colorPassMaterial:setFaceCull(sl.FaceCull.None)
    colorPassMaterial:bindParameter("matrices:wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)
    colorPassMaterial:bindParameter("matrices:model", sl.BindParameterSemantics.WorldMatrix)
    colorPassMaterial:setTextureParameter("mainTex", assetCache.textures.cobbleStone)
    colorPassMaterial:setTextureParameter("shadowMap", depthTex)

    local depthPassEffect = sl.Effect.createFromSource(dev, sl.generateEffectSource(depthPassEffectDesc))
    local depthPassMaterial = sl.Material.create(dev, depthPassEffect)
    depthPassMaterial:setFaceCull(sl.FaceCull.Back)
    depthPassMaterial:bindParameter("matrices:wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)

    local mainCam, camNode = createMainCamera(scene)
    camNode:findComponent("Transform"):setLocalPosition(vec3(5, 5, 5))
    camNode:findComponent("Transform"):lookAt(vec3(0, 0, 0), vec3(0, 1, 0))

    createSkybox(scene)
    local floorRenderer = createFloor(colorPassMaterial)
    local modelRenderer = createModel(colorPassMaterial)

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
        floorRenderer:setMaterial(0, depthPassMaterial)
        modelRenderer:setMaterial(0, depthPassMaterial)
        scene:visitByTags(~tags.skybox, renderCmp)
    end

    function renderMainCamFrame()
        floorRenderer:setMaterial(0, colorPassMaterial)
        modelRenderer:setMaterial(0, colorPassMaterial)
        scene:visitByTags(tags.skybox, renderCmp)
        scene:visitByTags(~tags.skybox, renderCmp)
    end

    function update()
        scene:visit(updateCmp)
        colorPassMaterial:setMatrixParameter("matrices:lightVp", lightCam:getViewProjectionMatrix())
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
