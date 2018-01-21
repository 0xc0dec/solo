--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local scene = sl.Scene.create(sl.device)

local node = scene:createNode()
assert(node)
scene:removeNode(node)

node = scene:createNode()
scene:removeNodeById(node:getId())

assert(scene:getDevice())
