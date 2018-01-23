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
    local logger = dev:getLogger()
    local fs = dev:getFileSystem()
    local scene = sl.Scene.create(dev)

    local createRotator = require "Rotator"
    local createMainCamera = require "MainCamera"
    local createSkybox = require "Skybox"
    local assetCache = (require "AssetCache")()
    local attachAxes = (require "Axes")(assetCache)
    local createFloor = require "Floor"
    local createCheckerBox = require "CheckerBox"
    local createDynamicQuad = require "DynamicQuad"
    local createTransparentQuad = require "TransparentQuad"
    local createLoadedMesh = require "LoadedMesh"
    local createTimeLabel = require "TimeLabel"
    local createSpawner = require "Spawner"
    local createMonitorQuad = require "MonitorQuad"
    local createOffscreenCamera = require "OffscreenCamera"
    local createPostProcessor = require "PostProcessor"
    local createTracer = require "Tracer"
    local createPostProcessorControlPanel = require "PostProcessorControlPanel"
    local tags = require "Tags"
    local effectTools = require "effect-tools"

    ---

    local effect = {
        vertex = {
            uniformBuffers = {
                ["matrices"] = {
                    wvp = "mat4"
                }
            },
    
            inputs = {
                position = "vec3",
                texCoord0 = "vec2"
            },
    
            outputs = {
                outTexCoord = "vec2"
            },
    
            entry = [[
                outTexCoord = texCoord0;
                outTexCoord.y = 1 - outTexCoord.y;
                gl_Position = #matrices:wvp# * vec4(position.xyz, 1.0);
            ]]
        },
    
        fragment = {
            samplers = {
                mainTex = "sampler2D"
            },
    
            outputs = {
                outFragColor = "vec4"
            },
    
            entry = [[
                outFragColor = texture(mainTex, outTexCoord);
            ]]
        }
    }

    local effectSrc = effectTools.generateEffectSource(effect, dev:getMode())
    local eff = sl.Effect.createFromSource(dev, effectSrc)

    local offscreenCamera, offscreenCameraTex = createOffscreenCamera(scene)
    offscreenCamera:setOrder(0)
    offscreenCamera:setTagMask(~(tags.monitor | tags.allPostProcessorSteps))

    local mainCamera, mainCameraNode = createMainCamera(scene, physics)
    local mainCameraTransform = mainCameraNode:findComponent("Transform")
    mainCameraTransform:setLocalPosition(vec3(0, 5, 10))
    mainCameraTransform:lookAt(vec3(0, 0, 0), vec3(0, 1, 0))
    mainCameraNode:addScriptComponent(createTracer(physics))
    mainCameraNode:addScriptComponent(createSpawner(assetCache))
    mainCameraNode:addScriptComponent(createPostProcessor(assetCache))
    mainCameraNode:addScriptComponent(createPostProcessorControlPanel(assetCache, mainCameraNode))
    mainCamera:setOrder(1)
    mainCamera:setTagMask(~tags.allPostProcessorSteps)

    createSkybox(scene)
    createCheckerBox(scene, assetCache)
    createFloor(scene, assetCache)
    createDynamicQuad(scene, assetCache)
    createLoadedMesh(scene, assetCache)
    createTimeLabel(scene)

    local monitorQuadParent = createMonitorQuad(scene, assetCache, offscreenCameraTex)
    attachAxes(monitorQuadParent)

    local transparentQuad = createTransparentQuad(scene, assetCache)
    attachAxes(transparentQuad)

    local rootNode = scene:createNode()
    attachAxes(rootNode)

    ---

    function shouldStop()
        return dev:isQuitRequested() or
               dev:isWindowCloseRequested() or
               dev:isKeyPressed(sl.KeyCode.Escape, true)
    end

    function run()
        while not shouldStop() do
            dev:update(scene)
            collectgarbage("collect")
        end
    end

    run()
end

callSafe(demo)
