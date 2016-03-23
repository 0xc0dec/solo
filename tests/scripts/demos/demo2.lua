function runDemo2()
	local device = solo.device
	local resMgr = device:getResourceManager()
	local scene = device:getScene()
	local logger = device:getLogger()

	local canvasSize = device:getCanvasSize()

	local shaders = dofile("../tests/scripts/demos/shaders.lua")
	local createEscapeWatcher = dofile("../tests/scripts/demos/escape-watcher.lua")
	local createMaterialTimeUpdater = dofile("../tests/scripts/demos/material-time-updater.lua")
	local createScreenshoter = dofile("../tests/scripts/demos/screenshoter.lua")
	local utils = dofile("../tests/scripts/demos/utils.lua")

	local camera

	function test()
		return {
			typeId = 2000,

			update = function()
				if device:isKeyPressed(solo.KeyCode.T, true) then
					resMgr:getOrLoadMeshAsync("../data/teapot.obj", function(mesh)
						print("Loaded")
					end)
				end
			end
		}
	end

	function createPostProcessor()
		return {
			typeId = 1000,

			init = function(self)
				self.fbTex1 = resMgr:getOrCreateTexture2D()
				self.fbTex1:setData(solo.ColorFormat.RGB, {}, canvasSize.x, canvasSize.y)
				self.fbTex1:setFiltering(solo.TextureFiltering.Nearest)
				self.fbTex1:setWrapping(solo.TextureWrapping.Clamp)
				self.fb1 = resMgr:getOrCreateFrameBuffer()
				self.fb1:setAttachments({ self.fbTex1 })
				camera:setRenderTarget(self.fb1)

				self.fbTex2 = resMgr:getOrCreateTexture2D()
				self.fbTex2:setData(solo.ColorFormat.RGB, {}, canvasSize.x, canvasSize.y)
				self.fbTex2:setFiltering(solo.TextureFiltering.Nearest)
				self.fbTex2:setWrapping(solo.TextureWrapping.Clamp)
				self.fb2 = resMgr:getOrCreateFrameBuffer()
				self.fb2:setAttachments({ self.fbTex2 })

				local grayscaleEffect = resMgr:getOrCreateEffect(shaders.vertex.passThrough, shaders.fragment.postProcessGrayscale)
				self.grayscaleMat = resMgr:getOrCreateMaterial(grayscaleEffect)
				self.grayscaleMat:setFloatParameter("rightSeparator", 0.25)

				local saturateEffect = resMgr:getOrCreateEffect(shaders.vertex.passThrough, shaders.fragment.postProcessSaturate)
				self.saturateMat = resMgr:getOrCreateMaterial(saturateEffect)
				self.saturateMat:setFloatParameter("leftSeparator", 0.75)
				self.saturateMat:setFloatParameter("rightSeparator", 1)

				local verticalBlurEffect = resMgr:getOrCreateEffect(shaders.vertex.passThrough, shaders.fragment.postProcessVerticalBlur)
				self.verticalBlurMat = resMgr:getOrCreateMaterial(verticalBlurEffect)
				self.verticalBlurMat:setFloatParameter("leftSeparator", 0.25)
				self.verticalBlurMat:setFloatParameter("rightSeparator", 0.75)

				local horizontalBlurEffect = resMgr:getOrCreateEffect(shaders.vertex.passThrough, shaders.fragment.postProcessHorizontalBlur)
				self.horizontalBlurMat = resMgr:getOrCreateMaterial(horizontalBlurEffect)
				self.horizontalBlurMat:setFloatParameter("leftSeparator", 0.25)
				self.horizontalBlurMat:setFloatParameter("rightSeparator", 0.75)
			end,

			onAfterCameraRender = function(self)
				-- bounce between the two frame buffers
				self.grayscaleMat:setTextureParameter("mainTex", self.fbTex1)
				device:getGraphics():blit(self.grayscaleMat, self.fb2)

				self.saturateMat:setTextureParameter("mainTex", self.fbTex2)
				device:getGraphics():blit(self.saturateMat, self.fb1)

				self.verticalBlurMat:setTextureParameter("mainTex", self.fbTex1)
				device:getGraphics():blit(self.verticalBlurMat, self.fb2)

				self.horizontalBlurMat:setTextureParameter("mainTex", self.fbTex2)
				device:getGraphics():blit(self.horizontalBlurMat, nil)
			end
		}
	end

	function initMesh()
		local tex = utils.loadTexture(resMgr, "../data/cobblestone.png")
		local effect = resMgr:getOrCreateEffect(shaders.vertex.wavy, shaders.fragment.texture)
		local mat = resMgr:getOrCreateMaterial(effect)
		mat:setPolygonFace(solo.PolygonFace.All)
		mat:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		mat:setTextureParameter("mainTex", tex)
		local mesh = resMgr:getOrLoadMesh("../data/monkey.obj")
		local node = scene:createNode()
		local renderer = node:addComponent("MeshRenderer")
		renderer:setMesh(mesh)
		renderer:setMaterial(0, mat)
		node:addScript(createMaterialTimeUpdater(device, "time"))
		logger:logInfo("Initialized mesh")
	end

	function initSkybox()
		utils.createSkybox(resMgr, scene, {
			"../data/skyboxes/deep-space/front.png",
			"../data/skyboxes/deep-space/back.png",
			"../data/skyboxes/deep-space/left.png",
			"../data/skyboxes/deep-space/right.png",
			"../data/skyboxes/deep-space/top.png",
			"../data/skyboxes/deep-space/bottom.png"
		})
		logger:logInfo("Initialized skybox")
	end

	function initCamera()
		local node = scene:createNode()
		local t = node:findComponent("Transform")
		t:setLocalPosition(solo.Vector3(0, 0, 10))
		node:addComponent("Spectator")
		camera = node:addComponent("Camera")
		camera:setClearColor(0, 0.6, 0.6, 1)
		camera:setNear(0.05)
		node:addScript(createEscapeWatcher(device))
		node:addScript(createPostProcessor())
		node:addScript(createScreenshoter(device, "demo2-screenshot.bmp"))
		node:addScript(test())
		logger:logInfo("Initialized camera")
	end

	initMesh()
	initSkybox()
	initCamera()

	device:run()
end

runDemo2()
