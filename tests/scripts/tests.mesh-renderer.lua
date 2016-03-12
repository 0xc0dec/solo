runTest(function()
	local scene = device:getScene()
	local mgr = device:getResourceManager()
	local mr = scene:createNode():addComponent("MeshRenderer")
	local eff = mgr:getOrCreateEffect("1", "2")
	local mat = mgr:getOrCreateMaterial(eff)
	local mesh = mgr:getOrCreateMesh("test/mesh-renderer")
	mr:setMesh(mesh)
	assert(mr:getMesh())
	mr:findMaterial(0)
	mr:getMaterialCount()
	mr:setMaterial(0, mat)
end, "MeshRenderer")