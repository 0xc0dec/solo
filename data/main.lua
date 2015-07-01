local device = solo.engine:getDevice()
local scene = solo.engine:getScene()

device:setWindowTitle("Set from script!")
print(device:getWindowTitle())

local node = scene:createNode()
print(node:getId())

--[[ print("1")
local a = solo.A()
print(a)
print("2")
print(a:func())
print("3")
]]--

print("1")
local a = solo.B()
print("2")
print(a:func())
print("3")