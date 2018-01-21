--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local scene = sl.Scene.create(sl.device)
local t1 = scene:createNode():findTransform()
local t2 = scene:createNode():findTransform()
local cam = scene:createNode():addCamera()
local v = sl.Vector3.new(1, 2, 3)
local q = sl.Quaternion.new()
local m = sl.Matrix.new()
local rad = sl.Radian.new(1)

t1:setParent(t2)
assert(t1:getParent())

assert(t2:getChild(0))
assert(t2:getChildrenCount())
t2:clearChildren()

t1:setParent(nil)

assert(t1:getWorldScale())
assert(t1:getLocalScale())

assert(t1:getWorldRotation())
assert(t1:getLocalRotation())

assert(t1:getWorldPosition())
assert(t1:getLocalPosition())

assert(t1:getWorldUp())
assert(t1:getLocalUp())

assert(t1:getWorldDown())
assert(t1:getLocalDown())

assert(t1:getWorldLeft())
assert(t1:getLocalLeft())

assert(t1:getWorldRight())
assert(t1:getLocalRight())

assert(t1:getWorldForward())
assert(t1:getLocalForward())

assert(t1:getWorldBack())
assert(t1:getLocalBack())

t1:translateLocal(v)
t1:scaleLocal(v)

t1:setLocalPosition(v)
t1:setLocalScale(v)

t1:rotate(q, sl.TransformSpace.World)
t1:rotateByAxisAngle(v, rad, sl.TransformSpace.World)

t1:setLocalRotation(q)
t1:setLocalAxisAngleRotation(v, rad)

t1:lookAt(v, v)

assert(t1:getMatrix())
assert(t1:getWorldMatrix())
assert(t1:getWorldViewMatrix(cam))
assert(t1:getWorldViewProjMatrix(cam))
assert(t1:getInvTransposedWorldViewMatrix(cam))
assert(t1:getInvTransposedWorldMatrix())

assert(t1:transformPoint(v))
assert(t1:transformDirection(v))
