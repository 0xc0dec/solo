--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local scene = sl.Scene.create(sl.device)
local n = scene:createNode()
local t = n:findTransform()

assert(t:getTypeId() ~= nil)
assert(t:getTag() ~= nil)
assert(t:getLayer() ~= nil)
t:setTag(123)
t:setLayer(10)
assert(t:getNode())