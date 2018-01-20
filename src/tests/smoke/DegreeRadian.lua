--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local rad1 = sl.Radian.new(1)
local rad2 = sl.Radian.new(sl.Degree.new(1))

assert(rad1:toRawRadian() ~= nil)
assert(rad1:toRawDegree() ~= nil)

assert(-rad1)
assert(rad1 + rad2)
assert(rad1 - rad2)
assert(rad1 * 2)
assert(rad1 / 2)

local deg1 = sl.Degree.new(2)
local deg2 = sl.Degree.new(sl.Radian.new(1))

assert(deg1:toRawRadian() ~= nil)
assert(deg1:toRawDegree() ~= nil)

assert(-deg1)
assert(deg1 + deg2)
assert(deg1 - deg2)
assert(deg1 * 2)
assert(deg1 / 2)