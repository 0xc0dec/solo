runTest(function()
	local m = device:getResourceManager():getOrCreateMesh("test/mesh")

	local l = solo.VertexBufferLayout()
	l:add(solo.VertexBufferLayoutSemantics.Position, 3)

	m:addBuffer(l, { 1, 2, 3 }, 1)
	m:addPart({ 1 }, 1)

	assert(m:getPartCount() == 1)

	m:removeBuffer(0)
	m:removePart(0)

	m:setPrimitiveType(solo.PrimitiveType.Lines)
	assert(m:getPrimitiveType() == solo.PrimitiveType.Lines)
end, "Mesh")
