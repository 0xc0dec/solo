--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local v1 = solo.Vector4(1, 2, 3, 4)
local v2 = solo.Vector4(1, 2, 3, 4)

assert(v1.x ~= nil)
assert(v1.y ~= nil)
assert(v1.z ~= nil)
assert(v1.w ~= nil)

assert(v1:isUnit() ~= nil)
assert(v1:isZero() ~= nil)

assert(solo.Vector4.angle(v1, v2) ~= nil)

v1:clamp(v1, v2)

assert(v1:distance(v2) ~= nil)
assert(v1:distanceSquared(v2) ~= nil)

assert(v1:dot(v2) ~= nil)
assert(solo.Vector4.dot(v1, v2) ~= nil)

assert(v1:length() ~= nil)
assert(v1:lengthSquared() ~= nil)

v1:normalize()
assert(v1:normalized())

assert(v1 + v2)
assert(v1 - v2)
assert(-v1)
assert(v1 * 2)
assert(v1 / 2)

--- GLM vector

local v1 = solo.Wector4(1, 2, 3, 4)
local v2 = solo.Wector4(-1, -2, -3, -4)

assert(v1:angle(v2) ~= nil)
assert(v1:dot(v2) ~= nil)
assert(v1:distance(v2) ~= nil)
assert(v1:length() ~= nil)
assert(v1:normalized() ~= nil)

assert(v1.x ~= nil)
assert(v1.y ~= nil)
assert(v1.z ~= nil)
assert(v1.w ~= nil)
assert(v1 + v2)
assert(v1 - v2)
assert(-v1)
assert(v1 * 2)
assert(v1 / 2)