return {
	loadTexture = function(resMgr, path)
		local texture = resMgr:getOrLoadTexture2D(path)
		texture:generateMipmaps()
		texture:setFiltering(solo.TextureFiltering.LinearMipmapNearest)
		texture:setAnisotropyLevel(8)
		return texture
	end,

	createSkybox = function(resMgr, scene, textures)
		local tex = resMgr:getOrLoadCubeTexture(textures)
		tex:setWrapping(solo.TextureWrapping.Clamp)
		tex:setFiltering(solo.TextureFiltering.Linear)
		local node = scene:createNode()
		local renderer = node:addComponent("SkyboxRenderer")
		renderer:setTexture(tex)
	end
}
