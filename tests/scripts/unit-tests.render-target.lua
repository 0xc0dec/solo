runTest(function()
	local rt = device:getResourceManager():getOrCreateRenderTarget("test/uri")
	assert(rt)
end, "RenderTarget")
