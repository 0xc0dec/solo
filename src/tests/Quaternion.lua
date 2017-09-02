--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local q = sl.Quaternion()

assert(q.x ~= nil)
assert(q.y ~= nil)
assert(q.z ~= nil)
assert(q.w ~= nil)

assert(sl.Quaternion.createFromAxisAngle(sl.Vector3(1, 2, 3), sl.Radian(1)))

assert(q:isIdentity() ~= nil)
assert(q:isZero() ~= nil)

q:conjugate()
q:inverse()

assert(q:normalized())
q:normalize()

local v = sl.Vector3(1, 2, 3)
assert(q:toAxisAngle(v))

assert(sl.Quaternion.lerp(q, q, 1))
assert(sl.Quaternion.slerp(q, q, 1))
assert(sl.Quaternion.squad(q, q, q, q, 1))

assert(q * sl.Quaternion())