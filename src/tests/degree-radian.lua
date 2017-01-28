local rad = solo.Radian(1)
assert(rad:toRawRadian() ~= nil)
assert(rad:toRawDegree() ~= nil)

local deg = solo.Degree(2)
assert(deg:toRawRadian() ~= nil)
assert(deg:toRawDegree() ~= nil)