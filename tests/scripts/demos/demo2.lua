return {
	shaders = dofile("../tests/scripts/demos/shaders.lua"),
	createEscapeWatcher = dofile("../tests/scripts/demos/escape-watcher.lua"),

	createPostProcessor = function(demo)
		return {
			typeId = 1000,

			init = function(self)
				local canvasSize = demo.device:getCanvasSize()
				local finalFbTex = demo.resMgr:getOrCreateTexture2D("/solo/demo2/post-processor/final-fb-tex")
				finalFbTex:setData(solo.ColorFormat.RGB, {}, canvasSize.x, canvasSize.y)
				finalFbTex:setFiltering(solo.TextureFiltering.Nearest)
				finalFbTex:setWrapping(solo.TextureWrapping.Clamp)
				self.finalFb = demo.resMgr:getOrCreateFrameBuffer("/solo/demo2/post-processor/final-fb")
				self.finalFb:setAttachments({ finalFbTex })

				local grayscaleEffect = demo.resMgr:getOrCreateEffect(demo.shaders.vertex.passThrough, demo.shaders.fragment.postProcessHalfGrayscale)
				self.grayscaleMat = demo.resMgr:getOrCreateMaterial(grayscaleEffect, "/solo/demo2/post-processor/grayscale-mat")
				self.grayscaleMat:setFloatParameter("rightSeparator", 0.2)
				self.grayscaleMat:setTextureParameter("mainTex", demo.fbTex)

				local saturateEffect = demo.resMgr:getOrCreateEffect(demo.shaders.vertex.passThrough, demo.shaders.fragment.postProcessHalfSaturate)
				self.saturateMat = demo.resMgr:getOrCreateMaterial(saturateEffect, "/solo/demo2/post-processor/saturate-mat")
				self.saturateMat:setFloatParameter("leftSeparator", 0.8)
				self.saturateMat:setFloatParameter("rightSeparator", 1)
				self.saturateMat:setTextureParameter("mainTex", finalFbTex)
			end,

			onAfterCameraRender = function(self)
				demo.device:getGraphics():blit(self.grayscaleMat, self.finalFb)
				demo.device:getGraphics():blit(self.saturateMat, nil)
			end
		}
	end,

	loadTexture = function(self, path)
		local texture = self.resMgr:getOrLoadTexture2D(path)
		texture:generateMipmaps()
		texture:setFiltering(solo.TextureFiltering.LinearMipmapNearest)
		texture:setAnisotropyLevel(8)
		return texture
	end,

	initMesh = function(self)
		local effect = self.resMgr:getOrCreateEffect(self.shaders.vertex.basic, self.shaders.fragment.texture)
		local mat = self.resMgr:getOrCreateMaterial(effect)
		local tex = self:loadTexture("../data/cobblestone.png")
		mat:setPolygonFace(solo.PolygonFace.All)
		mat:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		mat:setTextureParameter("mainTex", tex)
		local mesh = self.resMgr:getOrLoadMesh("../data/monkey.obj")
		local node = self.scene:createNode()
		local renderer = node:addComponent("MeshRenderer")
		renderer:setMesh(mesh)
		renderer:setMaterial(0, mat)
		self.logger:logInfo("Initialized mesh")
	end,

	initSkybox = function(self)
		local tex = self.resMgr:getOrLoadCubeTexture({
			"../data/skyboxes/deep-space/front.png",
			"../data/skyboxes/deep-space/back.png",
			"../data/skyboxes/deep-space/left.png",
			"../data/skyboxes/deep-space/right.png",
			"../data/skyboxes/deep-space/top.png",
			"../data/skyboxes/deep-space/bottom.png"
		}, "/solo/demo2/skybox")
		tex:setWrapping(solo.TextureWrapping.Clamp)
		tex:setFiltering(solo.TextureFiltering.Linear)

		local node = self.scene:createNode()
		local renderer = node:addComponent("SkyboxRenderer")
		renderer:setTexture(tex)

		self.logger:logInfo("Initialized skybox")
	end,

	initFrameBuffer = function(self)
		local canvasSize = self.device:getCanvasSize()
		self.fbTex = self.resMgr:getOrCreateTexture2D("/solo/demo2/fb-tex")
		self.fbTex:setData(solo.ColorFormat.RGB, {}, canvasSize.x, canvasSize.y)
		self.fbTex:setFiltering(solo.TextureFiltering.Nearest)
		self.fbTex:setWrapping(solo.TextureWrapping.Clamp)
		self.fb = self.resMgr:getOrCreateFrameBuffer("/solo/demo2/fb")
		self.fb:setAttachments({ self.fbTex })
		self.logger:logInfo("Initialized frame buffer")
	end,

	initCamera = function(self)
		local node = self.scene:createNode()
		local t = node:findComponent("Transform")
		t:setLocalPosition(solo.Vector3(0, 0, 10))
		node:addComponent("Spectator")
		node:addScript(self.createEscapeWatcher(self.device))
		node:addScript(self:createPostProcessor())
		local cam = node:addComponent("Camera")
		cam:setClearColor(0, 0.6, 0.6, 1)
		cam:setNear(0.05)
		cam:setRenderTarget(self.fb)
		self.logger:logInfo("Initialized camera")
	end,

	init = function(self, device)
		self.device = device
		self.resMgr = device:getResourceManager()
		self.scene = device:getScene()
		self.logger = device:getLogger()
		self:initFrameBuffer()
		self:initMesh()
		self:initSkybox()
		self:initCamera()
	end
}
