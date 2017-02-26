--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local m = solo.TransformMatrix()
local v = solo.Vector3(1, 2, 3)
local q = solo.Quaternion()
local rad = solo.Radian(1)
local bb = solo.BoundingBox(v, v)
local bs = solo.BoundingSphere(v, 1)
local plane = solo.Plane(v, 1)
local ray = solo.Ray(v, v)

assert(solo.TransformMatrix.createLookAt(v, v, v))
assert(solo.TransformMatrix.createPerspective(rad, 2, 3, 4))
assert(solo.TransformMatrix.createOrthographic(1, 2, 3, 4))
assert(solo.TransformMatrix.createScale(v))
assert(solo.TransformMatrix.createRotationFromQuaternion(q))
assert(solo.TransformMatrix.createRotationFromAxisAngle(v, rad))
assert(solo.TransformMatrix.createRotationX(rad))
assert(solo.TransformMatrix.createRotationY(rad))
assert(solo.TransformMatrix.createRotationZ(rad))
assert(solo.TransformMatrix.createTranslation(v))

assert(m:getScale() ~= nil)
assert(m:getRotation() ~= nil)
assert(m:getTranslation() ~= nil)

assert(m:getUpVector() ~= nil)
assert(m:getDownVector() ~= nil)
assert(m:getLeftVector() ~= nil)
assert(m:getRightVector() ~= nil)
assert(m:getForwardVector() ~= nil)
assert(m:getBackVector() ~= nil)

m:rotateByQuaternion(q)
m:rotateByAxisAngle(v, rad)
m:rotateX(rad)
m:rotateY(rad)
m:rotateZ(rad)

m:scaleByScalar(1)
m:scaleByVector(v)

m:translate(v)

assert(m:transformPoint(v))
assert(m:transformDirection(v))
assert(m:transformBoundingBox(bb))
assert(m:transformBoundingSphere(bs))
assert(m:transformPlane(plane))
assert(m:transformRay(ray))

m:decompose(v, q, v)