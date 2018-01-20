--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local v = sl.Vector3.new(1, 0, 0)
local ray = sl.Ray.new(v, v)

assert(ray:getOrigin() ~= nil)
ray:setOrigin(v)

assert(ray:getDirection() ~= nil)
ray:setDirection(v)
