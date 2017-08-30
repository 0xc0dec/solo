--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local m = solo.Matrix()
local v = solo.Vector3(1, 2, 3)
local q = solo.Quaternion()
local rad = solo.Radian(1)
local ray = solo.Ray(v, v)

assert(solo.Matrix.getSize() ~= nil)
assert(solo.Matrix.identity())
assert(solo.Matrix.createLookAt(v, v, v))
assert(solo.Matrix.createPerspective(rad, 2, 3, 4))
assert(solo.Matrix.createOrthographic(1, 2, 3, 4))
assert(solo.Matrix.createScale(v))
assert(solo.Matrix.createRotationFromQuaternion(q))
assert(solo.Matrix.createRotationFromAxisAngle(v, rad))
assert(solo.Matrix.createRotationX(rad))
assert(solo.Matrix.createRotationY(rad))
assert(solo.Matrix.createRotationZ(rad))
assert(solo.Matrix.createTranslation(v))

assert(m:isIdentity() ~= nil)

assert(m:getDeterminant() ~= nil)

assert(m:invert() ~= nil)
m:transpose()

assert(m * solo.Matrix())

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
assert(m:transformRay(ray))

m:decompose(v, q, v)