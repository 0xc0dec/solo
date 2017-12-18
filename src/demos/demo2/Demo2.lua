--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

package.path = "../../src/demos/demo2/?.lua;" .. package.path
package.path = "../../src/demos/common/?.lua;" .. package.path

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

    ---

    local tags = {
        skybox = 1 << 1,
        transparent = 1 << 2,
    }

    local camera, cameraNode = createMainCamera(scene)
    local cameraTransform = cameraNode:findComponent("Transform")
    cameraTransform:setLocalPosition(vec3(0, 6, 7))
    cameraTransform:lookAt(vec3(0, 0, 0), vec3(0, 1, 0))

    local cubeMesh = sl.Mesh.createFromPrefab(dev, sl.MeshPrefab.Cube)
    local quadMesh = sl.Mesh.createFromPrefab(dev, sl.MeshPrefab.Quad)

    createSkybox(scene, tags.skybox)
    createCheckerBox(scene, assetCache, cubeMesh)
    createFloor(scene, assetCache, cubeMesh)
    createDynamicQuad(scene, assetCache)

    local transparentQuad = createTransparentQuad(scene, assetCache, quadMesh, tags.transparent)
    attachAxes(transparentQuad)

    local rootNode = scene:createNode()
    attachAxes(rootNode)

    ---

    function keepRunning()
        return not dev:isQuitRequested() and
               not dev:isWindowCloseRequested() and
               not dev:isKeyPressed(sl.KeyCode.Escape, true)
    end

    function renderCmp(cmp)
        cmp:render()
    end

    function updateCmp(cmp)
        cmp:update()
    end

    function update()
        scene:visit(updateCmp)
    end

    function renderFrame()
        scene:visitByTags(tags.skybox, renderCmp)
        scene:visitByTags(~(tags.skybox | tags.transparent), renderCmp)
        scene:visitByTags(tags.transparent, renderCmp)
    end

    function render()
        camera:renderFrame(renderFrame)
    end

    function updateAndRender()
        physics:update()
        update()
        renderer:renderFrame(render)
    end

    function run()
        while keepRunning() do
            dev:update(updateAndRender)
            collectgarbage("collect")
        end
    end

    run()
end

callSafe(demo)
