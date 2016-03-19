runTest(function()
	local m = device:getResourceManager():getOrCreateMesh("test/mesh")

	local l = solo.VertexBufferLayout()
	l:add(solo.VertexBufferLayoutSemantics.Position, 3)

	assert(m:addVertexBuffer(l, { 1, 2, 3 }, 1) == 0)
	assert(m:addDynamicVertexBuffer(l, { 1, 2, 3 }, 1) == 1)
	assert(m:addPart({ 1 }, 1) == 0)

	m:updateDynamicVertexBuffer(1, 1, { 1, 2 }, 2)

	assert(m:getPartCount() == 1)

	m:removeVertexBuffer(0)
	m:removePart(0)

	m:setPrimitiveType(solo.PrimitiveType.Lines)
	assert(m:getPrimitiveType() == solo.PrimitiveType.Lines)
end, "Mesh")
