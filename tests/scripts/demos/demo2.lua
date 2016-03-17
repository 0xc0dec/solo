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

				local fb1Tex = demo.resMgr:getOrCreateTexture2D()
				fb1Tex:setData(solo.ColorFormat.RGB, {}, canvasSize.x, canvasSize.y)
				fb1Tex:setFiltering(solo.TextureFiltering.Nearest)
				fb1Tex:setWrapping(solo.TextureWrapping.Clamp)
				self.fb1 = demo.resMgr:getOrCreateFrameBuffer()
				self.fb1:setAttachments({ fb1Tex })

				local fb2Tex = demo.resMgr:getOrCreateTexture2D()
				fb2Tex:setData(solo.ColorFormat.RGB, {}, canvasSize.x, canvasSize.y)
				fb2Tex:setFiltering(solo.TextureFiltering.Nearest)
				fb2Tex:setWrapping(solo.TextureWrapping.Clamp)
				self.fb2 = demo.resMgr:getOrCreateFrameBuffer()
				self.fb2:setAttachments({ fb2Tex })

				local grayscaleEffect = demo.resMgr:getOrCreateEffect(demo.shaders.vertex.passThrough, demo.shaders.fragment.postProcessHalfGrayscale)
				self.grayscaleMat = demo.resMgr:getOrCreateMaterial(grayscaleEffect)
				self.grayscaleMat:setFloatParameter("rightSeparator", 0.2)
				self.grayscaleMat:setTextureParameter("mainTex", demo.fbTex) -- rendered from the main camera

				local saturateEffect = demo.resMgr:getOrCreateEffect(demo.shaders.vertex.passThrough, demo.shaders.fragment.postProcessHalfSaturate)
				self.saturateMat = demo.resMgr:getOrCreateMaterial(saturateEffect)
				self.saturateMat:setFloatParameter("leftSeparator", 0.8)
				self.saturateMat:setFloatParameter("rightSeparator", 1)
				self.saturateMat:setTextureParameter("mainTex", fb1Tex) -- rendered with grayscale

				local blurEffect = demo.resMgr:getOrCreateEffect(demo.shaders.vertex.passThrough, demo.shaders.fragment.postProcessBlur)
				self.blurMat = demo.resMgr:getOrCreateMaterial(blurEffect)
				self.blurMat:setFloatParameter("leftSeparator", 0.3)
				self.blurMat:setFloatParameter("rightSeparator", 0.7)
				self.blurMat:setTextureParameter("mainTex", fb2Tex) -- rendered with saturation
			end,

			onAfterCameraRender = function(self)
				demo.device:getGraphics():blit(self.grayscaleMat, self.fb1)
				demo.device:getGraphics():blit(self.saturateMat, self.fb2)
				demo.device:getGraphics():blit(self.blurMat, nil)
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
