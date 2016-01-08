runTest(function()
	local m = device:getResourceManager():getOrCreateMesh("test/uri")



	m:rebuildAsQuad()
	m:rebuildAsBox()
end, "Mesh")
