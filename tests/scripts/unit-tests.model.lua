runTest(function()
	local mgr = device:getResourceManager()
	local model = mgr:getOrCreateModel("dfg")
	local mesh = mgr:getOrCreateMesh("wer")

	assert(model)

	model:addMesh(mesh)
	assert(model:getMesh(0))
	assert(model:getMeshCount() == 1)
	model:removeMesh(mesh)
end, "Model")
