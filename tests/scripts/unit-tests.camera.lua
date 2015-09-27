runTest(function()
	local v = solo.Vector3(1, 2, 3)
	local q = solo.Quaternion()
	local scene = engine:getScene()
	local c = scene:createNode():addComponent("Camera")
	local rt = engine:getResourceManager():getOrCreateRenderTarget("abcd")
	c:setClearColor(1, 2, 3, 4)
	assert(c:getViewport())
	c:setViewport(1, 2, 3, 4)
	c:resetViewport()
	c:setPerspective(true)
	assert(c:isPerspective() ~= nil)
	assert(c:getNear() ~= nil)
	assert(c:getFar() ~= nil)
	assert(c:getFOV() ~= nil)
	assert(c:getWidth() ~= nil)
	assert(c:getHeight() ~= nil)
	assert(c:getAspectRatio() ~= nil)
	c:setNear(1)
	c:setFar(1)
	c:setFOV(1)
	c:setWidth(1)
	c:setHeight(1)
	c:setAspectRatio(1)
	c:getRenderTarget()
	c:setRenderTarget(rt)
end, "Camera")
