return {
	loadTexture = function(resMgr, path)
		local texture = resMgr:getOrLoadTexture2D(path)
		texture:generateMipmaps()
		texture:setFiltering(solo.TextureFiltering.LinearMipmapNearest)
		texture:setAnisotropyLevel(8)
		return texture
	end
}
