--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local v = solo.Vector3(1, 0, 0)
local ray = solo.Ray(v, v)

assert(ray:getOrigin() ~= nil)
ray:setOrigin(v)

assert(ray:getDirection() ~= nil)
ray:setDirection(v)
