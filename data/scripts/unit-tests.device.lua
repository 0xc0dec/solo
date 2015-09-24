runTest(function()
	local device = engine:getDevice()

	device:getWindowTitle()
	device:setWindowTitle("Solo engine window")
	device:getLifetime()
end, "Device")
