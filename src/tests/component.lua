local scene = solo.Scene.create(solo.device)
local n = scene:createNode()
local t = n:findComponent("Transform")

assert(t:getTypeId() ~= nil)
assert(t:getTags() ~= nil)
t:setTags(123)
assert(t:getNode())

t:update()
t:render(solo.RenderContext())