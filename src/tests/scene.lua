local scene = solo.device:getScene()

assert(scene:getDevice())
assert(scene:createNode())
scene:visit(function(cmp) end)
scene:visitByTags(1, function(cmp) end)