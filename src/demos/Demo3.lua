--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

package.path = "../../src/demos/?.lua;" .. package.path

require "Common"

function demo()
    local dev = sl.device
    local physics = dev:getPhysics()
    local fs = dev:getFileSystem()
    local scene = sl.Scene.create(dev)

    local tags = require "Tags"
    local createRotator = require "Rotator"
    local createMainCamera = require "MainCamera"
    local createSkybox = require "Skybox"
    local assetCache = (require "AssetCache")()

    ---

    local desc = {
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
                position = "vec4"
            },
    
            code = [[
                void main()
                {
                    gl_Position = #matrices:wvp# * vec4(sl_Position, 1);
                    SL_FIX_Y#gl_Position#;

                    position = gl_Position;
                }
            ]]
        },
    
        fragment = {
            samplers = {
            },
    
            outputs = {
                fragColor = { type = "vec4", target = 0 },
                fragPos = { type = "vec4", target = 1 }
            },
    
            code = [[
                void main()
                {
                    fragColor = vec4(1, 0, 0, 1);
                    fragPos = position;
                }
            ]]
        }
    }

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

    local canvasSize = dev:getCanvasSize()
    local posMap = sl.Texture2D.createEmpty(sl.device, canvasSize.x, canvasSize.y, sl.TextureFormat.RGBA16F)
    local albedoMap = sl.Texture2D.createEmpty(sl.device, canvasSize.x, canvasSize.y, sl.TextureFormat.RGBA8)
    local fb = sl.FrameBuffer.create(dev, { albedoMap, posMap })
    local effectSrc = sl.generateEffectSource(desc)
    print(effectSrc)
    local effect = sl.Effect.createFromSource(dev, effectSrc)
    local material = sl.Material.create(dev, effect)
    material:setFaceCull(sl.FaceCull.None)
    material:bindParameter("matrices:wvp", sl.BindParameterSemantics.WorldViewProjectionMatrix)

    local mainCam, camNode = createMainCamera(scene)
    camNode:findComponent("Transform"):setLocalPosition(vec3(5, 5, 5))
    camNode:findComponent("Transform"):lookAt(vec3(0, 0, 0), vec3(0, 1, 0))

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

    function renderMainCamFrame()
        scene:visitByTags(tags.skybox, renderCmp)
        scene:visitByTags(~tags.skybox, renderCmp)
    end

    function update()
        scene:visit(updateCmp)

        mainCam:setRenderTarget(fb)
        mainCam:renderFrame(renderMainCamFrame)

        mainCam:setRenderTarget(nil)
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
