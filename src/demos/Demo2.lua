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
    
            code = [[
                void main()
                {
                    gl_Position = #matrices:wvp# * vec4(sl_Position, 1);
                    SL_FIX_Y#gl_Position#;
                }
            ]]
        },
    
        fragment = {
            samplers = {
            },
    
            outputs = {
                fragColor = "vec4"
            },
    
            code = [[
                void main()
                {
                    fragColor = vec4(1, 0, 0, 1);
                }
            ]]
        }
    }

    local colorPassEffectDesc = {
        vertex = {
            uniformBuffers = {
                matrices = { -- TODO rename
                    wvp = "mat4",
                    model = "mat4",
                    lightVp = "mat4",
                    lightPos = "vec3"
                }
            },
    
            inputs = {
                sl_Position = "vec3",
                sl_TexCoord = "vec2",
                sl_Normal = "vec3"
            },
    
            outputs = {
                uv = "vec2",
                shadowCoord = "vec4",
                normal = "vec3",
                lightVec = "vec3",
                viewVec = "vec3"
            },
    
            code = [[
                void main()
                {
                    const mat4 biasMat = SL_SHADOW_BIAS_MAT;

                    uv = sl_TexCoord;
                    SL_FIX_UV#uv#;

                    gl_Position = #matrices:wvp# * vec4(sl_Position, 1);
                    SL_FIX_Y#gl_Position#;

                    vec4 lightProjectedPos = (#matrices:lightVp# * #matrices:model#) * vec4(sl_Position, 1.0);
                    SL_FIX_Y#lightProjectedPos#;
                    shadowCoord = biasMat * lightProjectedPos;

                    vec4 worldPos = #matrices:model# * vec4(sl_Position, 1.0);
                    normal = mat3(#matrices:model#) * sl_Normal;
                    lightVec = normalize(#matrices:lightPos# - sl_Position);
                    viewVec = -worldPos.xyz;			
                }
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
    
            code = [[
                const float ambient = 0.1;

                float sampleShadow(vec4 coords, vec2 offset)
                {
                    float shadow = 1.0;
                    float dist = texture(shadowMap, coords.st + offset).r;
                    if (dist < coords.z - 0.00002)
                        shadow = ambient;
                    return shadow;
                }

                float samplePCF(vec4 coords)
                {
                    ivec2 texDim = textureSize(shadowMap, 0);
                    float scale = 1.5;
                    float dx = scale * 1.0 / float(texDim.x);
                    float dy = scale * 1.0 / float(texDim.y);

                    float shadowFactor = 0.0;
                    int count = 0;
                    int range = 1;
                    
                    for (int x = -range; x <= range; x++)
                    {
                        for (int y = -range; y <= range; y++)
                        {
                            shadowFactor += sampleShadow(coords, vec2(dx * x, dy * y));
                            count++;
                        }
                    
                    }
                    return shadowFactor / count;
                }

                void main()
                {
                    vec3 n = normalize(normal);
                    vec3 l = normalize(lightVec);
                    vec3 v = normalize(viewVec);
                    vec3 r = normalize(-reflect(l, n));
                    float diffuse = max(dot(n, l), ambient);

                    vec4 coords = shadowCoord / shadowCoord.w;
                    float shadow = samplePCF(coords);

                    fragColor = texture(mainTex, uv) * min(diffuse, shadow);
                }
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
        cam:setFOV(sl.Radians.fromRawDegrees(50))
        cam:setViewport(vec4(0, 0, 2048, 2048))

        local depthTex = sl.Texture2D.createEmpty(sl.device, 2048, 2048, sl.TextureFormat.Depth)
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
    depthPassMaterial:setFaceCull(sl.FaceCull.None)
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
        colorPassMaterial:setVector3Parameter("matrices:lightPos", lightCamTransform:getWorldPosition())
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
