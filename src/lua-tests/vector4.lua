local v = sl.Vector4(1, 2, 3, 4)
assert(v)
assert(v.x == 1)
assert(v.y == 2)
assert(v.z == 3)
assert(v.w == 4)

assert(v:isUnit() ~= nil)
assert(v:isZero() ~= nil)
assert(v:distance(sl.Vector4(0, 0, 0, 0)) ~= nil)
assert(v:length() ~= nil)
assert(v:normalized() ~= nil)

v:normalize()

assert(v:angle(sl.Vector4(0, 0, 0, 0)) ~= nil)

v:clamp(sl.Vector4(0, 0, 0, 0), sl.Vector4(1, 1, 1, 1))

assert(v:dot(sl.Vector4(0, 0, 0, 0)) ~= nil)

assert(sl.Vector4(1, 1, 1, 1) + sl.Vector4(2, 2, 2, 2))
assert(sl.Vector4(1, 1, 1, 1) - sl.Vector4(2, 2, 2, 2))
assert(sl.Vector4(1, 1, 1, 1) * 12)
assert(sl.Vector4(1, 1, 1, 1) / 12)

assert(-sl.Vector4(1, 1, 1, 1))