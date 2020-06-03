local scene = sl.Scene.empty(sl.device)
assert(scene)

assert(scene:device() == sl.device)

local node = scene:createNode()
assert(node)

scene:removeNode(node)

node = scene:createNode()
scene:removeNodeById(node:id())

scene:visit(function(cmp) end)
scene:visitByTags(0, function(cmp) end)

scene:update()
scene:render(~0)