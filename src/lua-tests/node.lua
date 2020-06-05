local scene = sl.Scene.empty(sl.device)
local node = scene:createNode()

assert(node:id() ~= nil)
assert(node:scene() == scene)

local t = node:findComponent('Transform')
assert(t)

local cam = node:addComponent('Camera')
node:removeComponent('Camera')

assert(node:findComponent('Camera') == nil)

local cmp = node:addScriptComponent(sl.createComponent("TestyTest", {
    abc = 123,

    update = function(self)
    end
}))

cmp = node:findScriptComponent(sl.cmpId("TestyTest"))
-- findScriptComponent will always return a different wrapper, so direct comparison doesn't work
assert(cmp.abc == 123)

node:removeScriptComponent(cmp)
assert(node:findScriptComponent(sl.cmpId("TestyTest")) == nil)