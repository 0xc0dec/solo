--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

local scene = sl.Scene.create(sl.device)
local n = scene:createNode()

assert(n:getId() ~= nil)
assert(n:getScene())

local cmp = {
    typeId = 1,

    init = function()
    end,

    update = function()
    end,

    terminate = function()
    end
}

n:addScriptComponent(cmp)
assert(n:findScriptComponent(1))
n:removeScriptComponent(cmp)
assert(n:findScriptComponent(1) == nil)

assert(n:findComponent("Transform"))
assert(n:addComponent("Camera"))
n:removeComponent("Camera")
