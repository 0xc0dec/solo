return function (resMgr, scene)
	return {
		loadTexture = function(path, callback)
			local handle = resMgr:getOrLoadTexture2DAsync(path)
			resMgr:getOrLoadTexture2DAsync(path):done(function(texture)
				texture:generateMipmaps()
				texture:setFiltering(solo.TextureFiltering.LinearMipmapNearest)
				texture:setAnisotropyLevel(8)
				callback(texture)
			end)
		end,

		loadSkybox = function(textures, callback)
			resMgr:getOrLoadCubeTextureAsync(textures):done(function(tex)
				tex:setWrapping(solo.TextureWrapping.Clamp)
				tex:setFiltering(solo.TextureFiltering.Linear)
				local node = scene:createNode()
				local renderer = node:addComponent("SkyboxRenderer")
				renderer:setTexture(tex)
				callback(tex)
			end)
		end
	}
end
