--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local scene = sl.Scene.create(sl.device)
local cam = scene:createNode():addComponent("Camera")
local v4 = sl.Vector4(1, 2, 3, 4)

assert(cam:getTransform())

cam:setRenderTarget(sl.FrameBuffer.create(sl.device))
assert(cam:getRenderTarget() ~= nil)
cam:setRenderTarget(nil)

assert(cam:getClearColor())
cam:setClearColor(v4)

assert(cam:hasColorClearing() ~= nil)
cam:setColorClearing(true)

assert(cam:getViewport())
cam:setViewport(v4)

assert(cam:isPerspective() ~= nil)
cam:setPerspective(true)

assert(cam:getZNear() ~= nil)
cam:setZNear(1)

assert(cam:getZFar() ~= nil)
cam:setZFar(1)

assert(cam:getFOV() ~= nil)
cam:setFOV(sl.Radian(1))

assert(cam:getOrthoSize() ~= nil)
cam:setOrthoSize(sl.Vector2(1, 2))

assert(cam:getAspectRatio() ~= nil)

assert(cam:getTagMask() ~= nil)
cam:setTagMask(123)

assert(cam:getOrder() ~= nil)
cam:setOrder(3)

assert(cam:getViewMatrix())
assert(cam:getInvViewMatrix())
assert(cam:getProjectionMatrix())
assert(cam:getViewProjectionMatrix())
assert(cam:getInvViewProjectionMatrix())

assert(cam:windowPointToWorldRay(sl.Vector2(1, 2)))
