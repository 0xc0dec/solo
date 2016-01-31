runTest(function()
	local vfe = solo.VertexFormatElement(solo.VertexFormatElementSemantics.Position, 3, 1)
	assert(vfe.size == 3)
	assert(vfe.semantics == solo.VertexFormatElementSemantics.Position)
	assert(vfe.slot == 1)

	local vf = solo.VertexFormat({
		vfe,
		solo.VertexFormatElement(solo.VertexFormatElementSemantics.Normal, 3, 0)
	})
	assert(vf:getVertexSize(0) > 0)
	assert(vf:getVertexSize(1) > 0)
	assert(vf:getElementCount() == 2)
	vf:getElement(0)
end, "VertexFormat")
