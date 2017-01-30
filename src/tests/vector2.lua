local v1 = solo.Vector2(2, 3)
local v2 = solo.Vector2(4, 5)

assert(v1.x ~= nil)
assert(v1.y ~= nil)

assert(v1:isUnit() ~= nil)
assert(v1:isZero() ~= nil)

assert(solo.Vector2.angle(v1, v2) ~= nil)

v1:clamp(v1, v2)

assert(v1:distance(v2) ~= nil)
assert(v1:distanceSquared(v2) ~= nil)

assert(v1:dot(v2) ~= nil)
assert(solo.Vector2.dot(v1, v2) ~= nil)

assert(v1:length() ~= nil)
assert(v1:lengthSquared() ~= nil)

v1:normalize()
assert(v1:normalized() ~= nil)

assert(v1 + v2)
assert(v1 - v2)
assert(-v1)
assert(v1 * 2)
