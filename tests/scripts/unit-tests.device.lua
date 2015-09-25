runTest(function()
	assert(solo.KeyCode_A ~= nil)
	assert(solo.KeyCode_B ~= nil)
	assert(solo.KeyCode_C ~= nil)
	assert(solo.KeyCode_D ~= nil)
	assert(solo.KeyCode_E ~= nil)
	assert(solo.KeyCode_F ~= nil)
	assert(solo.KeyCode_G ~= nil)
	assert(solo.KeyCode_H ~= nil)
	assert(solo.KeyCode_I ~= nil)
	assert(solo.KeyCode_J ~= nil)
	assert(solo.KeyCode_K ~= nil)
	assert(solo.KeyCode_L ~= nil)
	assert(solo.KeyCode_M ~= nil)
	assert(solo.KeyCode_N ~= nil)
	assert(solo.KeyCode_O ~= nil)
	assert(solo.KeyCode_P ~= nil)
	assert(solo.KeyCode_Q ~= nil)
	assert(solo.KeyCode_R ~= nil)
	assert(solo.KeyCode_S ~= nil)
	assert(solo.KeyCode_T ~= nil)
	assert(solo.KeyCode_U ~= nil)
	assert(solo.KeyCode_V ~= nil)
	assert(solo.KeyCode_W ~= nil)
	assert(solo.KeyCode_X ~= nil)
	assert(solo.KeyCode_Y ~= nil)
	assert(solo.KeyCode_Z ~= nil);
	assert(solo.KeyCode_LeftArrow ~= nil);
	assert(solo.KeyCode_RightArrow ~= nil);
	assert(solo.KeyCode_UpArrow ~= nil);
	assert(solo.KeyCode_DownArrow ~= nil);
	assert(solo.KeyCode_Escape ~= nil);

	assert(solo.MouseButton_Left ~= nil);
	assert(solo.MouseButton_Middle ~= nil);
	assert(solo.MouseButton_Right ~= nil);

	local device = engine:getDevice()
	assert(device)
	device:setWindowTitle("Some title")
	assert(device:getWindowTitle() == "Some title")
	device:setCursorCaptured()
	assert(device:getCanvasSize() ~= nil)
	assert(device:getLifetime() ~= nil)
	assert(device:isKeyPressed(solo.KeyCode_A) ~= nil)
	assert(device:isKeyReleased(solo.KeyCode_A) ~= nil)
	assert(device:getMouseMotion())
	assert(device:isMouseButtonDown(solo.MouseButton_Left) ~= nil)
	assert(device:isMouseButtonReleased(solo.MouseButton_Left) ~= nil)
	assert(device:getTimeDelta() ~= nil)
	device:requestShutdown()
	assert(device:shutdownRequested() ~= nil)
end, "Device")
