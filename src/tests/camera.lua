local scene = solo.Scene.create(solo.device)
local cam = scene:createNode():addComponent("Camera")
local v = solo.Vector4(1, 2, 3, 4)
local rad = solo.Radian(1)
local fb = solo.FrameBuffer.create(solo.device)

cam:renderFrame(function(ctx) end)

assert(cam:getTransform())

cam:setRenderTarget(fb)
assert(cam:getRenderTarget() ~= nil)
cam:setRenderTarget(nil)

assert(cam:getClearColor())
cam:setClearColor(v)

assert(cam:isClearColorEnabled() ~= nil)
cam:setClearColorEnabled(true)

assert(cam:isClearDepthEnabled() ~= nil)
cam:setClearDepthEnabled(true)

assert(cam:getViewport())
cam:setViewport(v)

assert(cam:isPerspective() ~= nil)
cam:setPerspective(true)

assert(cam:getNear() ~= nil)
cam:setNear(1)

assert(cam:getFar() ~= nil)
cam:setFar(1)

assert(cam:getFOV() ~= nil)
cam:setFOV(rad)

assert(cam:getWidth() ~= nil)
cam:setWidth(1)

assert(cam:getHeight() ~= nil)
cam:setHeight(1)

assert(cam:getAspectRatio() ~= nil)
cam:setAspectRatio(1)

assert(cam:getViewMatrix())
assert(cam:getInvViewMatrix())
assert(cam:getProjectionMatrix())
assert(cam:getViewProjectionMatrix())
assert(cam:getInvViewProjectionMatrix())
