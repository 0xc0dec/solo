--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local scene = sl.Scene.create(sl.device)

assert(scene:getDevice())
assert(scene:createNode())
scene:visit(function(cmp) end)
scene:visitByTags(1, function(cmp) end)