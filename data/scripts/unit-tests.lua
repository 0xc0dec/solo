print("Unit tests started")

-- Some smoke testing...

local engine = solo.engine
local device = engine:getDevice()
local scene = engine:getScene()

-- Device
device:getWindowTitle()
device:setWindowTitle("Solo engine window")

-- Node
local node = scene:createNode()
node:getScene()
node:getId()
node:removeAllComponents()

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
		end,
	}
end

print(node:addComponent(create))


print("Unit tests finished")
