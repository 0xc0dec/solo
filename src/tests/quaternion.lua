local q = solo.Quaternion()

assert(q.x ~= nil)
assert(q.y ~= nil)
assert(q.z ~= nil)
assert(q.w ~= nil)

assert(solo.Quaternion.createFromAxisAngle(solo.Vector3(1, 2, 3), solo.Radian(1)))

assert(q:isIdentity() ~= nil)
assert(q:isZero() ~= nil)

q:conjugate()
q:inverse()

assert(q:normalized())
q:normalize()

local v = solo.Vector3(1, 2, 3)
assert(q:toAxisAngle(v))

assert(solo.Quaternion.lerp(q, q, 1))
assert(solo.Quaternion.slerp(q, q, 1))
assert(solo.Quaternion.squad(q, q, q, q, 1))

assert(q * solo.Quaternion())