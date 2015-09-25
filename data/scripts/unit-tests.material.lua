runTest(function()
	local mgr = engine:getResourceManager()
	local e = mgr:getOrCreateEffect("vs", "fs")
	local m = mgr:getOrCreateMaterial(e, "test/uri")
	assert(m:getParameter("param"))
	assert(m:getEffect())
end, "Material")
