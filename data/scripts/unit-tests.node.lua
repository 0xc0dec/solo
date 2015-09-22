print("Running Node tests...")

local scene = solo.engine:getScene()
local node = scene:createNode()

node:getScene()
node:getId()

function create()
	return {
		typeId = "TestComponent",

		init = function()
			print("Init called")
		end,

		update = function()
			print("Update called")
		end,

		render = function()
		end,

		postRender = function()
		end,

		terminate = function()
			print("terminate() called")
		end
	}
end

node:addComponent(create())

local cmp = create()
cmp.typeId = "AnotherComponent"
node:addComponent(cmp)

local cmp2 = create()
cmp2.typeId = "YetAnotherComponent"
node:addComponent(cmp2)

node:removeComponent("TestComponent")
node:removeComponent("AnotherComponent")
node:removeAllComponents()

print("Finished Node tests")
