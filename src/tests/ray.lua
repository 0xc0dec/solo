local v = solo.Vector3(1, 0, 0)
local ray = solo.Ray(v, v)
local plane = solo.Plane(v, 1)
local bb = solo.BoundingBox(v, v)
local bs = solo.BoundingSphere(v, 1)
local fr = solo.Frustum()

assert(ray:getOrigin() ~= nil)
ray:setOrigin(v)

assert(ray:getDirection() ~= nil)
ray:setDirection(v)

assert(ray:hitBoundingBox(bb) ~= nil)
assert(ray:hitBoundingSphere(bs) ~= nil)
assert(ray:hitFrustum(fr) ~= nil)
assert(ray:hitPlane(plane) ~= nil)