runTest(function()
	local l = solo.VertexBufferLayout()
	l:add(solo.VertexBufferLayoutSemantics.Position, 3)

	assert(l:getElementCount() == 1)
	assert(l:getSize() > 0)

	local el = l:getElement(0)
	assert(el.semantics == solo.VertexBufferLayoutSemantics.Position)
	assert(el.size == 3)
end, "VertexBufferLayout")
