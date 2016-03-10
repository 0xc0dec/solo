runTest(function()
	local logger = device:getLogger()
	logger:logDebug("Test debug message")
	logger:logInfo("Test Info message")
	logger:logWarning("Test warning message")
	logger:logError("Test error message")
	logger:logCritical("Test critical message")
	-- setTargetFile is already used in the test runner, so skip it here
end, "Logger")
