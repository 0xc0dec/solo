local device = solo.engine:getDevice()
local scene = solo.engine:getScene()

device:setWindowTitle("Set from script!")
print(device:getWindowTitle())

callback = {
	["update"] = function(dt)
		print("Updating")
	end
}

local node = scene:createNode()
print(node:getId())
node:addScriptComponent("callback")

print("Done")