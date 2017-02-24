--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local scene = solo.Scene.create(solo.device)
local n = scene:createNode()
local t = n:findComponent("Transform")

assert(t:getTypeId() ~= nil)
assert(t:getTag() ~= nil)
t:setTag(123)
assert(t:getNode())

t:update()
t:render(solo.RenderContext())