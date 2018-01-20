--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local v1 = sl.Vector3.new()
local v2 = sl.Vector3.new(1, 2, 3)
local v3 = sl.Vector3.new(1)

assert(v1.x ~= nil)
assert(v1.y ~= nil)
assert(v1.z ~= nil)

assert(v1:isUnit() ~= nil)
assert(v1:isZero() ~= nil)

assert(v1:distance(v2) ~= nil)
assert(v1:distanceSquared(v2) ~= nil)
assert(v1:length() ~= nil)
assert(v1:lengthSquared() ~= nil)
assert(v1:normalized() ~= nil)
v1:normalize()
assert(v1:angle(v2) ~= nil)
v1:clamp(v1, v2)
assert(v1:dot(v2) ~= nil)
assert(v1:cross(v2) ~= nil)

assert(v1 + v2)
assert(v1 - v2)
assert(-v1)
assert(v1 * 2)
assert(v1 / 2)