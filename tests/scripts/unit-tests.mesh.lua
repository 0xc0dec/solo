runTest(function()
	local m = device:getResourceManager():getOrCreateMesh("test/mesh")
	local vf = solo.VertexFormat({
		solo.VertexFormatElement(solo.VertexFormatElementSemantics.Position, 3)
	})

	m:resetVertexData(vf, { 1, 2, 3 }, 1, false)
	m:updateVertexData({ 2, 3, 4}, 1, 0)

	local part = m:addIndexedPart()
	assert(m:getPartCount() == 1)

	assert(m:getVertexFormat())

	m:setPrimitiveType(solo.MeshPrimitiveType.Lines)
	assert(m:getPrimitiveType() == solo.MeshPrimitiveType.Lines)

	m:rebuildAsQuad()
	m:rebuildAsBox()

	part:resetIndexData({ 1, 2, 3 }, 1, true)
	part:updateIndexData({ 2, 3, 4}, 1, 0)
	part:setPrimitiveType(solo.MeshPrimitiveType.Lines)
	assert(part:getPrimitiveType() == solo.MeshPrimitiveType.Lines)
end, "Mesh")
