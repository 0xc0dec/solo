runTest(function()
	local m = device:getResourceManager():getOrCreateMesh("test/uri")

	m:setVertices({
		solo.Vector3(1, 2, 3),
		solo.Vector3(1, 2, 3)
	})
	m:setUVs({
		solo.Vector2(1, 2),
		solo.Vector2(1, 2)
	})
	m:setNormals({
		solo.Vector3(1, 2, 3),
		solo.Vector3(1, 2, 3)
	})
	m:setIndices({
		0, 1
	})

	m:rebuildAsQuad()
	m:rebuildAsBox()
end, "Mesh")
