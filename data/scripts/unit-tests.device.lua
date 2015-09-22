print("Running Device tests...")

runTest(function()
	local engine = solo.engine
	local device = engine:getDevice()

	device:getWindowTitle()
	device:setWindowTitle("Solo engine window")
	device:getLifetime()
end)

print("Finished Device tests")
