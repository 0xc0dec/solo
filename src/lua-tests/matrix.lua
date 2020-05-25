local m = sl.Matrix()
assert(m)

assert(sl.Matrix.identity())

assert(m:isIdentity() ~= nil)
assert(m:getDeterminant() ~= nil)

m:invert()
m:transpose()

assert(sl.Matrix.createLookAt(sl.Vector3(0, 0, 0), sl.Vector3(1, 1, 1), sl.Vector3(0, 1, 0)))
assert(sl.Matrix.createPerspective(sl.Radians(2), 1, 2, 3))
assert(sl.Matrix.createOrthographic(1, 2, 3, 4))
assert(sl.Matrix.createScale(sl.Vector3(1, 2, 3)))
assert(sl.Matrix.createRotationFromQuaternion(sl.Quaternion()))
assert(sl.Matrix.createRotationFromAxisAngle(sl.Vector3(1, 2, 3), sl.Radians(12)))
assert(sl.Matrix.createTranslation(sl.Vector3(1, 2, 3)))

assert(m:scale())
assert(m:translation())
assert(m:rotation())
assert(m:upVector())
assert(m:downVector())
assert(m:leftVector())
assert(m:rightVector())
assert(m:forwardVector())
assert(m:backVector())

m:rotateByQuaternion(sl.Quaternion())
m:rotateByAxisAngle(sl.Vector3(1, 2, 3), sl.Radians(12))
m:scaleByScalar(12)
m:scaleByVector(sl.Vector3(1, 2, 3))
m:translate(sl.Vector3(1, 2, 3))

assert(m:transformPoint(sl.Vector3(1, 2, 3)))
assert(m:transformDirection(sl.Vector3(1, 2, 3)))
assert(m:transformRay(sl.Ray(sl.Vector3(0, 0, 0), sl.Vector3(1, 1, 1))))

local scale = sl.Vector3(1, 1, 1)
local rot = sl.Quaternion()
local translation = sl.Vector3(0, 0, 0)
m:decompose(scale, rot, translation)

assert(m * sl.Matrix())
