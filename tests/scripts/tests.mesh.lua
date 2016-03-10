runTest(function()
	local vf = solo.VertexFormat({
		solo.VertexFormatElement(solo.VertexFormatElementSemantics.Position, 3, 0)
	})
	local m = device:getResourceManager():getOrCreateMesh(vf, "test/mesh")

	m:resetData(0, { 1, 2, 3 }, 1, false)
	m:updateData(0, { 2, 3, 4}, 1, 0)

	local index = m:addIndex(MeshIndexFormat)
	assert(m:getIndexCount() == 1)

	assert(m:getVertexFormat())

	m:setPrimitiveType(solo.MeshPrimitiveType.Lines)
	assert(m:getPrimitiveType() == solo.MeshPrimitiveType.Lines)

	m:resetIndexData(0, { 1, 2, 3 }, 1, true)
	m:updateIndexData(0, { 2, 3, 4}, 1, 0)
	m:setIndexPrimitiveType(0, solo.MeshPrimitiveType.Lines)
	assert(m:getIndexPrimitiveType(0) == solo.MeshPrimitiveType.Lines)

	m:removeIndex(0)
end, "Mesh")
