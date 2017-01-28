local n = solo.device:getScene():createNode()

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

    onComponentAdded = function()
    end,

    onComponentRemoved = function()
    end
}

assert(n:getId() ~= nil)
assert(n:getScene())

n:addScriptComponent(cmp)
n:removeScriptComponent(cmp)

assert(n:findComponent("Transform"))
assert(n:addComponent("Camera"))
n:removeComponent("Camera")