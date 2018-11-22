--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local createRotator = require "rotator"

return function(scene)
    local node = scene:createNode()

    local cam = node:addComponent("Camera")
    cam:setZNear(0.05)
    cam:setZFar(1000)
    cam:setFieldOfView(sl.Radians.fromRawDegrees(50))
    cam:setViewport(vec4(0, 0, 2048, 2048))

    local depthTex = sl.Texture2D.empty(sl.device, 2048, 2048, sl.TextureFormat.Depth24)
    depthTex:setFilter(sl.TextureFilter.Nearest, sl.TextureFilter.Nearest, sl.TextureMipFilter.Nearest)

    local fb = sl.FrameBuffer.fromAttachments(sl.device, { depthTex })
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