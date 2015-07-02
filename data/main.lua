--[[ local engine = solo.engine
local device = engine:getDevice()
local scene = engine:getScene()
local resManager = engine:getResourceManager()

device:setWindowTitle("Set from script!")
print("Window title is: " .. device:getWindowTitle())

local node = scene:createNode()

callback = 
{
	["update"] = function(dt)
	end
}
node:addScript("callback")

local tex = resManager:getOrLoadTexture("../data/freeman1.png")
print(tex)

print("Initialized")
]]--

local test = Test.new()
print(test:getInt())
print(test:getString())
print(test:acceptString("wewer"))