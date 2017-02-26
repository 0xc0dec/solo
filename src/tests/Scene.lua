--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local scene = solo.Scene.create(solo.device)

assert(scene:getDevice())
assert(scene:createNode())
scene:visit(function(cmp) end)
scene:visitByTags(1, function(cmp) end)