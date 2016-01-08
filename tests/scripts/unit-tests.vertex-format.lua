runTest(function()
	local vfe = solo.VertexFormatElement(solo.VertexFormatElementSemantics.Position, 3)
	assert(vfe.size == 3)
	assert(vfe.semantics == solo.VertexFormatElementSemantics.Position)

	local vf = solo.VertexFormat({
		vfe,
		solo.VertexFormatElement(solo.VertexFormatElementSemantics.Normal, 3)
	})
	assert(vf:getVertexSize() > 0)
	assert(vf:getElementCount() == 2)
	vf:getElement(0)
end, "VertexFormat")
