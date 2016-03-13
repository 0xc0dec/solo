runTest(function()
	local mgr = device:getResourceManager()
	local e = mgr:getOrCreateEffect("vs", "fs")
	local m = mgr:getOrCreateMaterial(e)

	assert(m:getEffect())

	assert(m:getPolygonFace() == solo.PolygonFace.CW)
	m:setPolygonFace(solo.PolygonFace.All)

	assert(m:isDepthWriteEnabled() == true)
	m:setDepthWriteEnabled(false)

	assert(m:isDepthTestEnabled() == true)
	m:setDepthTestEnabled(false)

	assert(m:getDepthPassFunction() ~= nil)
	m:setDepthPassFunction(solo.DepthPassFunction.Always)

	local v2 = solo.Vector2.unit()
	local v3 = solo.Vector3.unit()
	local v4 = solo.Vector4.unit()
	local mat = solo.Matrix()
	local tex = mgr:getOrCreateTexture2D()

	m:setFloatParameter("float", 1)
	m:setFloatArrayParameter("floatArr", {1, 2, 3})
	m:setVector2Parameter("v2", v2)
	m:setVector2ArrayParameter("v2Arr", {v2})
	m:setVector3Parameter("v3", v3)
	m:setVector3ArrayParameter("v3Arr", {v3})
	m:setVector4Parameter("v4", v4)
	m:setVector4ArrayParameter("v4Arr", {v4})
	m:setMatrixParameter("mat", mat)
	m:setMatrixArrayParameter("matArray", {mat})
	m:setTextureParameter("tex", tex)
	m:setParameterAutoBinding("auto", solo.AutoBinding.CameraWorldPosition)

end, "Material")
