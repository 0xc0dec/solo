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

    onComponentAdded = function(cmp)
    end,

    onComponentRemoved = function(cmp)
    end
}

assert(n:getId() ~= nil)
assert(n:getScene())

n:addScriptComponent(cmp)
n:removeScriptComponent(cmp)

assert(n:findComponent("Transform"))
assert(n:addComponent("Camera"))
n:removeComponent("Camera")

local t = n:findComponent("Transform")
assert(t:getTypeId() ~= nil)
assert(t:getTags() ~= nil)
t:setTags(123)
assert(t:getNode())
