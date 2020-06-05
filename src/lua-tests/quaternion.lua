local q = sl.Quaternion()
assert(q)
assert(q.x == 0)
assert(q.y == 0)
assert(q.z == 0)
assert(q.w == 1)

assert(sl.Quaternion.fromAxisAngle(sl.Vector3(1, 2, 3), sl.Radians(12)))
assert(sl.Quaternion.lerp(sl.Quaternion(), sl.Quaternion(), 1))
assert(sl.Quaternion.slerp(sl.Quaternion(), sl.Quaternion(), 1))

assert(q:isIdentity() ~= nil)
assert(q:isZero() ~= nil)

q:conjugate()
q:invert()
assert(q:inverted())
q:normalize()
assert(q:normalized() ~= nil)

local v = sl.Vector3(1, 2, 3)
assert(q:toAxisAngle(v) ~= nil)

assert(sl.Quaternion() * sl.Quaternion())