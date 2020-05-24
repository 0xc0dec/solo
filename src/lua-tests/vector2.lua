local v1 = sl.Vector2(1, 2)
assert(v1)
assert(v1.x == 1)
assert(v1.y == 2)

assert(v1:isUnit() ~= nil)
assert(v1:isZero() ~= nil)
assert(v1:distance(sl.Vector2(0, 0)) ~= nil)
assert(v1:length() ~= nil)
assert(v1:normalized() ~= nil)

v1:normalize()

assert(v1:angle(sl.Vector2(0, 0)) ~= nil)

v1:clamp(sl.Vector2(0, 0), sl.Vector2(1, 1))

assert(v1:dot(sl.Vector2(0, 0)) ~= nil)

assert(sl.Vector2(1, 1) + sl.Vector2(2, 2))
assert(sl.Vector2(1, 1) - sl.Vector2(2, 2))
assert(sl.Vector2(1, 1) * 12)
assert(sl.Vector2(1, 1) / 12)

assert(-sl.Vector2(1, 1))