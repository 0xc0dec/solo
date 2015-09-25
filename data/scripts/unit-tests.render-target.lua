runTest(function()
	local rt = engine:getResourceManager():getOrCreateRenderTarget("test/uri")
	assert(rt)
end, "RenderTarget")
