local v = sl.Vector2(1, 2)
assert(v)
assert(v.x == 1)
assert(v.y == 2)

assert(v:isUnit() ~= nil)
assert(v:isZero() ~= nil)
assert(v:distance(sl.Vector2(0, 0)) ~= nil)
assert(v:length() ~= nil)
assert(v:normalized() ~= nil)

v:normalize()

assert(v:angle(sl.Vector2(0, 0)) ~= nil)

v:clamp(sl.Vector2(0, 0), sl.Vector2(1, 1))

assert(v:dot(sl.Vector2(0, 0)) ~= nil)

assert(sl.Vector2(1, 1) + sl.Vector2(2, 2))
assert(sl.Vector2(1, 1) - sl.Vector2(2, 2))
assert(sl.Vector2(1, 1) * 12)
assert(sl.Vector2(1, 1) / 12)

assert(-sl.Vector2(1, 1))