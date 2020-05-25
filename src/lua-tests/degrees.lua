local d = sl.Degrees(1)
assert(d)

assert(d:toRawRadians() ~= nil)
assert(d:toRawDegrees() == 1)

assert(sl.Degrees.fromRadians(sl.Radians(12)) ~= nil)
assert(sl.Degrees.fromRawRadians(12) ~= nil)

assert(-sl.Degrees(12))
assert(sl.Degrees(12) + sl.Degrees(23))
assert(sl.Degrees(12) - sl.Degrees(23))
assert(sl.Degrees(12) * 2)
assert(sl.Degrees(12) / 2)