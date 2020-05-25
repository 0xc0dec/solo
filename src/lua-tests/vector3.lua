local v = sl.Vector3(1, 2, 3)
assert(v)
assert(v.x == 1)
assert(v.y == 2)
assert(v.z == 3)

assert(v:isUnit() ~= nil)
assert(v:isZero() ~= nil)
assert(v:distance(sl.Vector3(0, 0, 0)) ~= nil)
assert(v:length() ~= nil)
assert(v:normalized() ~= nil)

v:normalize()

assert(v:angle(sl.Vector3(0, 0, 0)) ~= nil)

v:clamp(sl.Vector3(0, 0, 0), sl.Vector3(1, 1, 1))

assert(v:dot(sl.Vector3(0, 0, 0)) ~= nil)

assert(v:cross(sl.Vector3(1, 2, 3)) ~= nil)

assert(sl.Vector3(1, 1, 1) + sl.Vector3(2, 2, 2))
assert(sl.Vector3(1, 1, 1) - sl.Vector3(2, 2, 2))
assert(sl.Vector3(1, 1, 1) * 12)
assert(sl.Vector3(1, 1, 1) / 12)

assert(-sl.Vector3(1, 1, 1))