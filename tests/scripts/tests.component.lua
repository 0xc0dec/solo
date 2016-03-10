runTest(function()
	local scene = device:getScene()
	local c = scene:createNode():addComponent("Camera")

	c:getRenderQueue()
	c:setRenderQueue(10)
	c:getTags()
	c:getNode()
end, "Component")
