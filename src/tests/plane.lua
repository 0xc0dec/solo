--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local p = solo.Plane(solo.Vector3(1, 2, 3), 1)
local v = solo.Vector3(1, 2, 3)
local bs = solo.BoundingSphere(v, 1)
local bb = solo.BoundingBox(v, v)
local fr = solo.Frustum()
local ray = solo.Ray(v, v)

assert(p:getNormal())
p:setNormal(v)

assert(p:getDistance() ~= nil)
p:setDistance(1)

assert(p:getDistanceToPoint(v) ~= nil)

assert(solo.Plane.getCommonPoint(p, p, p))

assert(p:intersectBoundingBox(bb))
assert(p:intersectBoundingSphere(bs))
assert(p:intersectFrustum(fr))
assert(p:intersectPlane(p))
assert(p:intersectRay(ray))

assert(p:isParallel(p) ~= nil)