--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local m = sl.Matrix()
local v = sl.Vector3(1, 2, 3)
local q = sl.Quaternion()
local rad = sl.Radians(1)
local ray = sl.Ray(v, v)

assert(sl.Matrix.identity())
assert(sl.Matrix.createLookAt(v, v, v))
assert(sl.Matrix.createPerspective(rad, 2, 3, 4))
assert(sl.Matrix.createOrthographic(1, 2, 3, 4))
assert(sl.Matrix.createScale(v))
assert(sl.Matrix.createRotationFromQuaternion(q))
assert(sl.Matrix.createRotationFromAxisAngle(v, rad))
assert(sl.Matrix.createTranslation(v))

assert(m:isIdentity() ~= nil)

assert(m:getDeterminant() ~= nil)

m:invert()
m:transpose()

assert(m * sl.Matrix())

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

m:scaleByScalar(1)
m:scaleByVector(v)

m:translate(v)

assert(m:transformPoint(v))
assert(m:transformDirection(v))
assert(m:transformRay(ray))

m:decompose(v, q, v)