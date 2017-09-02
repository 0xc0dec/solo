--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local scene = sl.Scene.create(sl.device)
local cam = scene:createNode():addComponent("Camera")
local v4 = sl.Vector4(1, 2, 3, 4)

cam:renderFrame(function(ctx) end)

assert(cam:getTransform())

cam:setRenderTarget(sl.FrameBuffer.create(sl.device))
assert(cam:getRenderTarget() ~= nil)
cam:setRenderTarget(nil)

assert(cam:getClearColor())
cam:setClearColor(v4)

assert(cam:isClearColorEnabled() ~= nil)
cam:setClearColorEnabled(true)

assert(cam:isClearDepthEnabled() ~= nil)
cam:setClearDepthEnabled(true)

assert(cam:getViewport())
cam:setViewport(v4)

assert(cam:isPerspective() ~= nil)
cam:setPerspective(true)

assert(cam:getNear() ~= nil)
cam:setNear(1)

assert(cam:getFar() ~= nil)
cam:setFar(1)

assert(cam:getFOV() ~= nil)
cam:setFOV(sl.Radian(1))

assert(cam:getOrthoSize() ~= nil)
cam:setOrthoSize(sl.Vector2(1, 2))

assert(cam:getAspectRatio() ~= nil)
cam:setAspectRatio(1)

assert(cam:getViewMatrix())
assert(cam:getInvViewMatrix())
assert(cam:getProjectionMatrix())
assert(cam:getViewProjectionMatrix())
assert(cam:getInvViewProjectionMatrix())
