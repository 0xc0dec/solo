runTest(function()
	local scene = device:getScene()
	local node = scene:createNode()
	local cam = node:addComponent("Camera")

	assert(scene:getDevice())
	assert(scene:createNode())

	scene:renderWithCamera(cam)
	scene:removeAllComponents(123)
	scene:clear()
	scene:update()
	scene:render()
end, "Scene")
