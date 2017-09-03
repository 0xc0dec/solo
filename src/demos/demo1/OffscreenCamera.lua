--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return function(scene)
	local canvasSize = sl.device:getCanvasSize()

    local tex = sl.RectTexture.create(sl.device,
        math.floor(canvasSize.x / 8.0), math.floor(canvasSize.y / 8.0), sl.TextureFormat.RGB)
    tex:setFiltering(sl.TextureFiltering.Nearest)
    tex:setWrapping(sl.TextureWrapping.Clamp)

    local node = scene:createNode()
    node:findComponent("Transform"):setLocalPosition(vec3(0, 0, 10))

    local cam = node:addComponent("Camera")
    cam:setClearColor(vec4(1, 0, 1, 1))
    cam:setNear(0.05)
    cam:setViewport(vec4(0, 0, canvasSize.x / 8, canvasSize.y / 8))

    local fb = sl.FrameBuffer.create(sl.device)
    fb:setAttachments({ tex })
    cam:setRenderTarget(fb)

    return cam, tex
end