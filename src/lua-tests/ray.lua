local r = sl.Ray(sl.Vector3(0, 0, 0), sl.Vector3(1, 1, 1))
assert(r)

assert(r:origin())
r:setOrigin(sl.Vector3(-1, -1, -1))

assert(r:direction())
r:setDirection(sl.Vector3(0, 1, 1))