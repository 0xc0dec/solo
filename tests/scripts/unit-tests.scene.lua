runTest(function()
	local scene = device:getScene()

	assert(scene:getDevice())
	assert(scene:createNode())

	scene:clearNodeComponents(123)
	scene:clear()
	scene:update()
	scene:render()
end, "Scene")
