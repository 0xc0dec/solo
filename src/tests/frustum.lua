--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local f = solo.Frustum()
local m = solo.Matrix()
local v = solo.Vector3(1, 1, 1)
local bb = solo.BoundingBox(v, v)
local bs = solo.BoundingSphere(v, 1)
local ray = solo.Ray(v, v)
local plane = solo.Plane(v, 1)

assert(f:getNearPlane())
assert(f:getFarPlane())
assert(f:getLeftPlane())
assert(f:getRightPlane())
assert(f:getTopPlane())
assert(f:getBottomPlane())

assert(f:getMatrix())
f:setMatrix(m)

assert(f:getCorners())
assert(f:getNearCorners())
assert(f:getFarCorners())

assert(f:intersectsPoint(v) ~= nil)
assert(f:intersectsBoundingBox(bb) ~= nil)
assert(f:intersectsBoundingSphere(bs) ~= nil)

assert(f:hitByRay(ray) ~= nil)
assert(f:intersectPlane(plane) ~= nil)
