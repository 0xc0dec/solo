runTest(function()
	local v = solo.Vector3(1, 2, 3)
	local q = solo.Quaternion()
	local scene = device:getScene()
	local c = scene:createNode():addComponent("Camera")
	local rt = device:getResourceManager():getOrCreateRenderTarget("abcd")

	assert(c:getRenderOrder() ~= nil)
	c:setRenderOrder(10)

	assert(c:getClearColor())
	c:setClearColor(1, 2, 3, 4)

	assert(c:getViewport())
	c:setViewport(1, 2, 3, 4)
	c:resetViewport()

	assert(c:isPerspective() ~= nil)
	c:setPerspective(true)

	assert(c:getNear() ~= nil)
	c:setNear(1)

	assert(c:getFar() ~= nil)
	c:setFar(1)

	assert(c:getFOV() ~= nil)
	c:setFOV(1)

	assert(c:getWidth() ~= nil)
	c:setWidth(1)

	assert(c:getHeight() ~= nil)
	c:setHeight(1)

	assert(c:getAspectRatio() ~= nil)
	c:setAspectRatio(1)

	c:getRenderTarget()
	c:setRenderTarget(rt)
end, "Camera")
