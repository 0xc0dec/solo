local scene = sl.Scene.empty(sl.device)
local node = scene:createNode()
local cam = node:addComponent('Camera')

cam:renderFrame(function() end)

assert(cam:transform())

local tex = sl.Texture2D.empty(sl.device, 100, 100, sl.TextureFormat.Depth24)
local fb = sl.FrameBuffer.fromAttachments(sl.device, { tex })
cam:setRenderTarget(fb)
assert(cam:renderTarget() == fb)

assert(cam:clearColor())
cam:setClearColor(sl.Vector4(0.1, 0.1, 0.1, 1))

assert(cam:hasColorClearing() ~= nil)
cam:setColorClearing(true)

assert(cam:viewport())
cam:setViewport(sl.Vector4(0, 0, 100, 100))

assert(cam:zNear())
cam:setZNear(123)

assert(cam:zFar())
cam:setZFar(123)

assert(cam:fieldOfView())
cam:setFieldOfView(sl.Radians(12))

assert(cam:orthoSize())
cam:setOrthoSize(sl.Vector2(1, 2))

assert(cam:windowPointToWorldRay(sl.Vector2(1, 2)))
assert(cam:aspectRatio())
assert(cam:viewMatrix())
assert(cam:invViewMatrix())
assert(cam:projectionMatrix())
assert(cam:viewProjectionMatrix())
assert(cam:invViewProjectionMatrix())

assert(node:findComponent('Camera') == node:findComponent('Camera'))