local device = solo.engine:getDevice()
local scene = solo.engine:getScene()

device:setWindowTitle("Set from script!")
print(device:getWindowTitle())

local node = scene:createNode()
print(node:getId())
node:addScriptComponent("lalala")

print("Done")