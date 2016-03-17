return {
	shaders = dofile("../tests/scripts/demos/shaders.lua"),
	createEscapeWatcher = dofile("../tests/scripts/demos/escape-watcher.lua"),
	createMaterialTimeUpdater = dofile("../tests/scripts/demos/material-time-updater.lua"),
	utils = dofile("../tests/scripts/demos/utils.lua"),

	createPostProcessor = function(demo)
		return {
			typeId = 1000,

			init = function(self)
				local canvasSize = demo.device:getCanvasSize()

				self.fbTex1 = demo.resMgr:getOrCreateTexture2D()
				self.fbTex1:setData(solo.ColorFormat.RGB, {}, canvasSize.x, canvasSize.y)
				self.fbTex1:setFiltering(solo.TextureFiltering.Nearest)
				self.fbTex1:setWrapping(solo.TextureWrapping.Clamp)
				self.fb1 = demo.resMgr:getOrCreateFrameBuffer()
				self.fb1:setAttachments({ self.fbTex1 })
				demo.camera:setRenderTarget(self.fb1)

				self.fbTex2 = demo.resMgr:getOrCreateTexture2D()
				self.fbTex2:setData(solo.ColorFormat.RGB, {}, canvasSize.x, canvasSize.y)
				self.fbTex2:setFiltering(solo.TextureFiltering.Nearest)
				self.fbTex2:setWrapping(solo.TextureWrapping.Clamp)
				self.fb2 = demo.resMgr:getOrCreateFrameBuffer()
				self.fb2:setAttachments({ self.fbTex2 })

				local grayscaleEffect = demo.resMgr:getOrCreateEffect(demo.shaders.vertex.passThrough, demo.shaders.fragment.postProcessGrayscale)
				self.grayscaleMat = demo.resMgr:getOrCreateMaterial(grayscaleEffect)
				self.grayscaleMat:setFloatParameter("rightSeparator", 0.25)

				local saturateEffect = demo.resMgr:getOrCreateEffect(demo.shaders.vertex.passThrough, demo.shaders.fragment.postProcessSaturate)
				self.saturateMat = demo.resMgr:getOrCreateMaterial(saturateEffect)
				self.saturateMat:setFloatParameter("leftSeparator", 0.25)
				self.saturateMat:setFloatParameter("rightSeparator", 0.5)

				local verticalBlurEffect = demo.resMgr:getOrCreateEffect(demo.shaders.vertex.passThrough, demo.shaders.fragment.postProcessVerticalBlur)
				self.verticalBlurMat = demo.resMgr:getOrCreateMaterial(verticalBlurEffect)
				self.verticalBlurMat:setFloatParameter("leftSeparator", 0.5)
				self.verticalBlurMat:setFloatParameter("rightSeparator", 0.75)

				local horizontalBlurEffect = demo.resMgr:getOrCreateEffect(demo.shaders.vertex.passThrough, demo.shaders.fragment.postProcessHorizontalBlur)
				self.horizontalBlurMat = demo.resMgr:getOrCreateMaterial(horizontalBlurEffect)
				self.horizontalBlurMat:setFloatParameter("leftSeparator", 0.5)
				self.horizontalBlurMat:setFloatParameter("rightSeparator", 0.75)
			end,

			onAfterCameraRender = function(self)
				-- bounce between the two frame buffers
				self.grayscaleMat:setTextureParameter("mainTex", self.fbTex1)
				demo.device:getGraphics():blit(self.grayscaleMat, self.fb2)

				self.saturateMat:setTextureParameter("mainTex", self.fbTex2)
				demo.device:getGraphics():blit(self.saturateMat, self.fb1)

				self.verticalBlurMat:setTextureParameter("mainTex", self.fbTex1)
				demo.device:getGraphics():blit(self.verticalBlurMat, self.fb2)

				self.horizontalBlurMat:setTextureParameter("mainTex", self.fbTex2)
				demo.device:getGraphics():blit(self.horizontalBlurMat, nil)
			end
		}
	end,

	initMesh = function(self)
		local tex = self.utils.loadTexture(self.resMgr, "../data/cobblestone.png")
		local effect = self.resMgr:getOrCreateEffect(self.shaders.vertex.wavy, self.shaders.fragment.texture)
		local mat = self.resMgr:getOrCreateMaterial(effect)
		mat:setPolygonFace(solo.PolygonFace.All)
		mat:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		mat:setTextureParameter("mainTex", tex)
		local mesh = self.resMgr:getOrLoadMesh("../data/monkey.obj")
		local node = self.scene:createNode()
		local renderer = node:addComponent("MeshRenderer")
		renderer:setMesh(mesh)
		renderer:setMaterial(0, mat)
		node:addScript(self.createMaterialTimeUpdater(self.device, "time"))
		self.logger:logInfo("Initialized mesh")
	end,

	initSkybox = function(self)
		self.utils.createSkybox(self.resMgr, self.scene, {
			"../data/skyboxes/deep-space/front.png",
			"../data/skyboxes/deep-space/back.png",
			"../data/skyboxes/deep-space/left.png",
			"../data/skyboxes/deep-space/right.png",
			"../data/skyboxes/deep-space/top.png",
			"../data/skyboxes/deep-space/bottom.png"
		})
		self.logger:logInfo("Initialized skybox")
	end,

	initFrameBuffer = function(self)

		self.logger:logInfo("Initialized frame buffer")
	end,

	initCamera = function(self)
		local node = self.scene:createNode()
		local t = node:findComponent("Transform")
		t:setLocalPosition(solo.Vector3(0, 0, 10))
		node:addComponent("Spectator")
		self.camera = node:addComponent("Camera")
		self.camera:setClearColor(0, 0.6, 0.6, 1)
		self.camera:setNear(0.05)
		node:addScript(self.createEscapeWatcher(self.device))
		node:addScript(self:createPostProcessor())
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
