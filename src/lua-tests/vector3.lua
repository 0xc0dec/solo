local v1 = sl.Vector3(1, 2, 3)
assert(v1)
assert(v1.x == 1)
assert(v1.y == 2)
assert(v1.z == 3)

assert(v1:isUnit() ~= nil)
assert(v1:isZero() ~= nil)
assert(v1:distance(sl.Vector3(0, 0, 0)) ~= nil)
assert(v1:length() ~= nil)
assert(v1:normalized() ~= nil)

v1:normalize()

assert(v1:angle(sl.Vector3(0, 0, 0)) ~= nil)

v1:clamp(sl.Vector3(0, 0, 0), sl.Vector3(1, 1, 1))

assert(v1:dot(sl.Vector3(0, 0, 0)) ~= nil)

assert(v1:cross(sl.Vector3(1, 2, 3)) ~= nil)

assert(sl.Vector3(1, 1, 1) + sl.Vector3(2, 2, 2))
assert(sl.Vector3(1, 1, 1) - sl.Vector3(2, 2, 2))
assert(sl.Vector3(1, 1, 1) * 12)
assert(sl.Vector3(1, 1, 1) / 12)

assert(-sl.Vector3(1, 1, 1))