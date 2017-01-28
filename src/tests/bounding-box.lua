local v = solo.Vector3(1, 2, 3)
local bb = solo.BoundingBox(v, v)
local bs = solo.BoundingSphere(v, 1)

assert(bb.min ~= nil)
assert(bb.max ~= nil)

assert(bb:getCenter())
assert(bb:getCorners())

assert(bb:intersectsBoundingSphere(bs) ~= nil)
assert(bb:intersectsBoundingBox(bb) ~= nil)
assert(bb:intersectsFrustum(solo.Frustum()) ~= nil)

assert(bb:hitByRay(solo.Ray(v, v)) ~= nil)

assert(bb:intersectPlane(solo.Plane(v, 1)) ~= nil)

assert(bb:isEmpty() ~= nil)

bb:mergeBoundingBox(bb)
bb:mergeBoundingSphere(bs)