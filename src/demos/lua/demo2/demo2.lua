--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

sl.includeDir("../../src/demos/lua/common")
sl.includeDir("../../src/demos/lua/demo2")

require "common"

function demo()
    local dev = sl.device
    local physics = dev:physics()
    local fs = dev:fileSystem()
    local scene = sl.Scene.empty(dev)

    local tags = require "tags"
    local createRotator = require "rotator"
    local createMainCamera = require "main-camera"
    local assetCache = (require "asset-cache")()

    ---

    local mrtEffectDesc = {
        vertex = {
            uniformBuffers = {
                matrices = {
                    world = "mat4",
                    wvp = "mat4"
                }
            },
    
            inputs = {
                sl_Position = "vec3",
                sl_TexCoord = "vec2",
                sl_Normal = "vec3",
                sl_Tangent = "vec3"
            },
    
            outputs = {
                position = "vec4",
                uv = "vec2",
                normal = "vec3",
                tangent = "vec3"
            },
    
            code = [[
                void main()
                {
                    gl_Position = #matrices:wvp# * vec4(sl_Position, 1);
                    SL_FIX_Y#gl_Position#;

                    position = #matrices:world# * vec4(sl_Position, 1);
                    SL_FIX_Y#position#;

                    uv = sl_TexCoord;
                    SL_FIX_UV#uv#;

                    mat3 transpInvWorld = transpose(inverse(mat3(#matrices:world#)));
                    normal = transpInvWorld * normalize(sl_Normal);
                    tangent = transpInvWorld * normalize(sl_Tangent);
                }
            ]]
        },
    
        fragment = {
            samplers = {
                mainTex = "sampler2D",
                normalTex = "sampler2D"
            },
    
            outputs = {
                fragColor = { type = "vec4", target = 0 },
                fragPos = { type = "vec4", target = 1 },
                fragNormal = { type = "vec3", target = 2 },
                fragTangent = { type = "vec3", target = 3 }
            },
    
            code = [[
                void main()
                {
                    fragColor = texture(mainTex, uv);

                    fragPos = position;

                    vec3 n = normalize(normal);
                    vec3 t = normalize(tangent);
                    vec3 b = cross(n, t);
                    mat3 tbn = mat3(t, b, n);
                    fragNormal = tbn * normalize(texture(normalTex, uv).xyz * 2.0 - vec3(1.0));
                }
            ]]
        }
    }

    local deferEffectDesc = {
        vertex = {
            inputs = {
                sl_Position = "vec3",
                sl_TexCoord = "vec2"
            },
    
            outputs = {
                position = "vec4",
                uv = "vec2"
            },
    
            code = [[
                void main()
                {
                    gl_Position = vec4(sl_Position, 1);
                    SL_FIX_Y#gl_Position#;

                    uv = sl_TexCoord;
                    SL_FIX_UV#uv#;
                }
            ]]
        },
    
        fragment = {
            samplers = {
                albedoMap = "sampler2D",
                positionMap = "sampler2D",
                normalMap = "sampler2D"
            },

            uniformBuffers = {
                uniforms = {
                    lightPos = "vec3",
                    eyePos = "vec3"
                }
            },
    
            outputs = {
                fragColor = { type = "vec4", target = 0 }
            },
    
            code = [[
                void main()
                {
                    vec4 albedo = texture(albedoMap, uv);
                    vec3 position = texture(positionMap, uv).rgb;
                    vec3 normal = normalize(texture(normalMap, uv).rgb);

                    vec3 toLight = normalize(#uniforms:lightPos# - position);
                    vec3 toEye = normalize(#uniforms:eyePos# - position);
                    float ndotl = max(0.0, dot(normal, toLight));

                    vec3 r = reflect(-toLight, normal);
                    float ndotr = max(0.0, dot(r, toEye));
                    vec3 spec = vec3(1, 1, 1) * pow(ndotr, 16.0);

                    fragColor = albedo * ndotl + vec4(spec, 1);
                }
            ]]
        }
    }

    function createModel(material)
        local node = scene:createNode()
    
        local renderer = node:addComponent("MeshRenderer")
        renderer:setMaterial(0, material)
    
        local transform = node:findComponent("Transform")
        transform:setLocalPosition(vec3(0, -1, 0))
    
        local layout = sl.VertexBufferLayout()
        layout:addAttribute(sl.VertexAttributeSemantics.Position)
        layout:addAttribute(sl.VertexAttributeSemantics.Normal)
        layout:addAttribute(sl.VertexAttributeSemantics.TexCoord)
        layout:addAttribute(sl.VertexAttributeSemantics.Tangent)
        sl.Mesh.fromFileAsync(dev, getAssetPath("meshes/Teapot.obj"), layout):done(
            function(mesh)
                renderer:setMesh(mesh)
            end)

        return renderer
    end

    local canvasSize = dev:canvasSize()

    local bricksTex = sl.Texture2D.fromFile(sl.device, getAssetPath("textures/rock_color.jpg"), true)
    bricksTex:setAnisotropyLevel(16)

    local bricksNormalTex = sl.Texture2D.fromFile(sl.device, getAssetPath("textures/rock_normal.jpg"), true)
    bricksNormalTex:setAnisotropyLevel(16)
    
    local positionTex = sl.Texture2D.empty(sl.device, canvasSize.x, canvasSize.y, sl.TextureFormat.RGBA16F)
    local normalTex = sl.Texture2D.empty(sl.device, canvasSize.x, canvasSize.y, sl.TextureFormat.RGBA16F)
    local tangentTex = sl.Texture2D.empty(sl.device, canvasSize.x, canvasSize.y, sl.TextureFormat.RGBA16F)
    local albedoTex = sl.Texture2D.empty(sl.device, canvasSize.x, canvasSize.y, sl.TextureFormat.RGBA8)
    local mrtFrameBuffer = sl.FrameBuffer.create(dev, { albedoTex, positionTex, normalTex, tangentTex })
    local mrtEffectDesc = sl.generateEffectSource(mrtEffectDesc)
    local mrtEffect = sl.Effect.fromSource(dev, mrtEffectDesc)
    local mrtMaterial = sl.Material.fromEffect(dev, mrtEffect)
    mrtMaterial:setFaceCull(sl.FaceCull.Back)
    mrtMaterial:bindParameter("matrices:wvp", sl.ParameterBinding.WorldViewProjectionMatrix)
    mrtMaterial:bindParameter("matrices:world", sl.ParameterBinding.WorldMatrix)
    mrtMaterial:setTextureParameter("mainTex", bricksTex)
    mrtMaterial:setTextureParameter("normalTex", bricksNormalTex)

    local deferEffectSrc = sl.generateEffectSource(deferEffectDesc)
    local deferEffect = sl.Effect.fromSource(dev, deferEffectSrc)
    local deferMaterial = sl.Material.fromEffect(dev, deferEffect)
    deferMaterial:setFaceCull(sl.FaceCull.None)
    deferMaterial:setTextureParameter("albedoMap", albedoTex)
    deferMaterial:setTextureParameter("positionMap", positionTex)
    deferMaterial:setTextureParameter("normalMap", normalTex)
    deferMaterial:setVector3Parameter("uniforms:lightPos", vec3(10, 10, 10))
    deferMaterial:bindParameter("uniforms:eyePos", sl.ParameterBinding.CameraWorldPosition)

    local deferQuadNode = scene:createNode()
    local deferQuadRenderer = deferQuadNode:addComponent("MeshRenderer")
    deferQuadRenderer:setTag(tags.postProcessorStep) -- TODO use other tag
    deferQuadRenderer:setMesh(assetCache.meshes.getQuad())
    deferQuadRenderer:setMaterial(0, deferMaterial);

    local mainCam, camNode = createMainCamera(scene)
    camNode:findComponent("Transform"):setLocalPosition(vec3(5, 5, 5))
    camNode:findComponent("Transform"):lookAt(vec3(0, 0, 0), vec3(0, 1, 0))

    createModel(mrtMaterial)

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

    function renderScene()
        scene:visitByTags(tags.skybox, renderCmp)
        scene:visitByTags(~(tags.skybox | tags.postProcessorStep), renderCmp)
    end

    function renderQuad()
        scene:visitByTags(tags.postProcessorStep, renderCmp)
    end

    function update()
        scene:visit(updateCmp)

        mainCam:setRenderTarget(mrtFrameBuffer)
        mainCam:renderFrame(renderScene)

        mainCam:setRenderTarget(nil)
        mainCam:renderFrame(renderQuad)
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
