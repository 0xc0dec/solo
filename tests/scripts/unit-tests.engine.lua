runTest(function()
	assert(solo.EngineMode_Stub)
	assert(solo.EngineMode_OpenGL)

	assert(solo.EngineCreationArgs())
	assert(solo.EngineCreationArgs(solo.EngineMode_OpenGL))
	assert(solo.EngineCreationArgs(solo.EngineMode_OpenGL, 1))
	assert(solo.EngineCreationArgs(solo.EngineMode_OpenGL, 1, 2))
	assert(solo.EngineCreationArgs(solo.EngineMode_OpenGL, 1, 2, 3))
	assert(solo.EngineCreationArgs(solo.EngineMode_OpenGL, 1, 2, 3, 4))
	assert(solo.EngineCreationArgs(solo.EngineMode_OpenGL, 1, 2, 3, 4, true))
	assert(solo.EngineCreationArgs(solo.EngineMode_OpenGL, 1, 2, 3, 4, true, "Test title"))

	local args = solo.EngineCreationArgs(solo.EngineMode_OpenGL, 1, 2, 3, 4, true, "Test title")
	assert(args.mode)
	assert(args.bits)
	assert(args.canvasWidth)
	assert(args.canvasHeight)
	assert(args.depth)
	assert(args.fullScreen)
	assert(args.windowTitle == "Test title")

	assert(engine:getDevice())
	assert(engine:getScene())
	assert(engine:getResourceManager())
	assert(engine:getFileSystem())
	assert(engine:getMode() ~= nil)

	engine:setCallback(
	{
		onDeviceCloseRequested = function()
			return true
		end,

		onEngineStarted = function()
		end,

		onEngineStopped = function()
		end
	})

	engine:setCallback({})
end, "Engine")
