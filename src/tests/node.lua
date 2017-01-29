local scene = solo.Scene.create(solo.device)
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
    end,

    render = function(ctx)
    end,

    onComponentAdded = function(cmp)
    end,

    onComponentRemoved = function(cmp)
    end
}

n:addScriptComponent(cmp)
n:removeScriptComponent(cmp)

assert(n:findComponent("Transform"))
assert(n:addComponent("Camera"))
n:removeComponent("Camera")
