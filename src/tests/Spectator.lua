--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local scene = solo.Scene.create(solo.device)
local s = scene:createNode():addComponent("Spectator")

assert(s:getMovementSpeed() ~= nil)
s:setMovementSpeed(1)

assert(s:getMouseSensitivity() ~= nil)
s:setMouseSensitivity(1)

assert(s:getRotationAcceleration() ~= nil)
s:setRotationAcceleration(1)