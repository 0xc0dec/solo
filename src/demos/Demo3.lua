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

    local mainCam, camNode = createMainCamera(scene)
    camNode:findComponent("Transform"):setLocalPosition(vec3(5, 5, 5))
    camNode:findComponent("Transform"):lookAt(vec3(0, 0, 0), vec3(0, 1, 0))

    createSkybox(scene)

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
