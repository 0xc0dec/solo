--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(dev, scene)
	local canvasSize = dev:getCanvasSize()

    local tex = solo.RectTexture.create(dev)
    tex:setData(solo.ImageFormat.RGB, {}, math.floor(canvasSize.x / 8.0), math.floor(canvasSize.y / 8.0))
    tex:setFiltering(solo.TextureFiltering.Nearest)
    tex:setWrapping(solo.TextureWrapping.Clamp)

    local node = scene:createNode()
    node:findComponent("Transform"):setLocalPosition(vec3(0, 0, 10))

    local cam = node:addComponent("Camera")
    cam:setClearColor(vec4(1, 0, 1, 1))
    cam:setNear(0.05)
    cam:setViewport(vec4(0, 0, canvasSize.x / 8, canvasSize.y / 8))

    local fb = solo.FrameBuffer.create(dev)
    fb:setAttachments({ tex })
    cam:setRenderTarget(fb)

    return cam, tex
end