runTest(function()
	local mgr = device:getResourceManager()
	local e = mgr:getOrCreateEffect("vs", "fs")
	local m = mgr:getOrCreateMaterial(e, "test/uri")
	local mp = m:getParameter("param")
	assert(mp)
	assert(m:getEffect())

	assert(m:getPolygonFace() == solo.PolygonFace_CW)
	m:setPolygonFace(solo.PolygonFace_All)

	assert(m:isDepthWriteEnabled() == true)
	m:setDepthWriteEnabled(false)

	assert(m:isDepthTestEnabled() == true)
	m:setDepthTestEnabled(false)

	assert(m:getDepthPassFunction() ~= nil)
	m:setDepthPassFunction(solo.DepthPassFunction_Always)

	local v2 = solo.Vector2.unit()
	local v3 = solo.Vector3.unit()
	local v4 = solo.Vector4.unit()
	local mat = solo.Matrix()
	local tex = mgr:getOrCreateTexture2D()

	mp:setFloat(1)
	mp:setFloatArray({1, 2, 3})
	mp:setInt(1)
	mp:setIntArray({1, 2, 3})
	mp:setVector2(v2)
	mp:setVector2Array({v2})
	mp:setVector3(v3)
	mp:setVector3Array({v3})
	mp:setVector4(v4)
	mp:setVector4Array({v4})
	mp:setMatrix(mat)
	mp:setMatrixArray({mat})
	mp:setTexture(tex)
	mp:setTextureArray({tex})
	mp:setFunction(function (var, renderContext) end)

	mp:bindValue(solo.AutoBinding_CameraWorldPosition)

end, "Material")
