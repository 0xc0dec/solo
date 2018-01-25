--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local rad1 = sl.Radians(1)
local rad2 = sl.Radians(1)

assert(rad1:toRawRadians() ~= nil)
assert(rad1:toRawDegrees() ~= nil)

assert(-rad1)
assert(rad1 + rad2)
assert(rad1 - rad2)
assert(rad1 * 2)
assert(rad1 / 2)


local deg1 = sl.Degrees(2)
local deg2 = sl.Degrees(2)

assert(deg1:toRawRadians() ~= nil)
assert(deg1:toRawDegrees() ~= nil)

assert(-deg1)
assert(deg1 + deg2)
assert(deg1 - deg2)
assert(deg1 * 2)
assert(deg1 / 2)