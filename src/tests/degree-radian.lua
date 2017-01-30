local rad1 = solo.Radian(1)
local rad2 = solo.Radian(1)

assert(rad1:toRawRadian() ~= nil)
assert(rad1:toRawDegree() ~= nil)

assert(-rad1)
assert(rad1 + rad2)
assert(rad1 - rad2)
assert(rad1 * 2)
assert(rad1 / 2)


local deg1 = solo.Degree(2)
local deg2 = solo.Degree(2)

assert(deg1:toRawRadian() ~= nil)
assert(deg1:toRawDegree() ~= nil)

assert(-deg1)
assert(deg1 + deg2)
assert(deg1 - deg2)
assert(deg1 * 2)
assert(deg1 / 2)