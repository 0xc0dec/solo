local r = sl.Radians(1)
assert(r)

assert(r:toRawRadians() == 1)
assert(r:toRawDegrees() ~= nil)

assert(sl.Radians.fromDegrees(sl.Degrees(12)) ~= nil)
assert(sl.Radians.fromRawDegrees(12) ~= nil)

assert(-sl.Radians(12))
assert(sl.Radians(12) + sl.Radians(23))
assert(sl.Radians(12) - sl.Radians(23))
assert(sl.Radians(12) * 2)
assert(sl.Radians(12) / 2)