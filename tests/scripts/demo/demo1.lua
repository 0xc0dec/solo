return {
	shaders = dofile("../tests/scripts/demo/shaders.lua"),
	components = dofile("../tests/scripts/demo/components.lua"),
	renderTargetQuadTag = 2,

	loadTexture = function(self, path)
		local texture = self.resourceManager:getOrLoadTexture2D(path)
		texture:generateMipmaps()
		texture:setFiltering(solo.TextureFiltering.LinearMipmapNearest)
		texture:setAnisotropyLevel(8)
		return texture
	end,

	initTextures = function(self)
		local canvasSize = self.device:getCanvasSize()

		local mainCameraRTT = self.resourceManager:getOrCreateTexture2D("/solo/demo/main-camera-rtt")
		mainCameraRTT:setData(solo.ColorFormat.RGB, {}, canvasSize.x, canvasSize.y)
		mainCameraRTT:setFiltering(solo.TextureFiltering.Nearest)
		mainCameraRTT:setWrapping(solo.TextureWrapping.Clamp)

		local offscreenCameraRTT = self.resourceManager:getOrCreateTexture2D("/solo/demo/offscreen-camera-rtt")
		offscreenCameraRTT:setData(solo.ColorFormat.RGB, {}, math.floor(canvasSize.x / 8), math.floor(canvasSize.y / 8))
		offscreenCameraRTT:setFiltering(solo.TextureFiltering.Nearest)
		offscreenCameraRTT:setWrapping(solo.TextureWrapping.Clamp)

		self.textures =
		{
			tex1 = self:loadTexture("../data/freeman1.png"),
			tex2 = self:loadTexture("../data/cobblestone.png"),
			mainCameraRTT = mainCameraRTT,
			offscreenCameraRTT = offscreenCameraRTT,
		}

		self.logger:logInfo("Initialized textures")
	end,

	initMaterials = function(self)
		local simpleTextureEffect = self.resourceManager:getOrCreateEffect(self.shaders.vertex.basic, self.shaders.fragment.texture)
		local wavySimpleTextureEffect = self.resourceManager:getOrCreateEffect(self.shaders.vertex.wavy, self.shaders.fragment.texture)
		local colorEffect = self.resourceManager:getOrCreateEffect(self.shaders.vertex.basic, self.shaders.fragment.color)
		local checkerEffect = self.resourceManager:getOrCreateEffect(self.shaders.vertex.basic, self.shaders.fragment.checker)
		local texWithLightingEffect = self.resourceManager:getOrCreateEffect(self.shaders.vertex.basicLighting, self.shaders.fragment.textureWithLighting)

		local redMaterial = self.resourceManager:getOrCreateMaterial(colorEffect)
		redMaterial:setPolygonFace(solo.PolygonFace.All)
		redMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		redMaterial:setVector4Parameter("color", solo.Vector4(1, 0, 0, 1))

		local greenMaterial = self.resourceManager:getOrCreateMaterial(colorEffect)
		greenMaterial:setPolygonFace(solo.PolygonFace.All)
		greenMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		greenMaterial:setVector4Parameter("color", solo.Vector4(0, 1, 0, 1))

		local blueMaterial = self.resourceManager:getOrCreateMaterial(colorEffect)
		blueMaterial:setPolygonFace(solo.PolygonFace.All)
		blueMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		blueMaterial:setVector4Parameter("color", solo.Vector4(0, 0, 1, 1))

		local whiteMaterial = self.resourceManager:getOrCreateMaterial(colorEffect)
		whiteMaterial:setPolygonFace(solo.PolygonFace.All)
		whiteMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		whiteMaterial:setVector4Parameter("color", solo.Vector4(1, 1, 1, 1))

		local simpleTextureMaterial = self.resourceManager:getOrCreateMaterial(simpleTextureEffect)
		simpleTextureMaterial:setPolygonFace(solo.PolygonFace.All)
		simpleTextureMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		simpleTextureMaterial:setTextureParameter("mainTex", self.textures.tex1)

		local wavySimpleTextureMaterial = self.resourceManager:getOrCreateMaterial(wavySimpleTextureEffect)
		wavySimpleTextureMaterial:setPolygonFace(solo.PolygonFace.CCW)
		wavySimpleTextureMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		wavySimpleTextureMaterial:setTextureParameter("mainTex", self.textures.tex2)
		wavySimpleTextureMaterial:setFloatParameter("time", 0)

		local checkerMaterial = self.resourceManager:getOrCreateMaterial(checkerEffect)
		checkerMaterial:setPolygonFace(solo.PolygonFace.All)
		checkerMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix);
		checkerMaterial:setVector4Parameter("color", solo.Vector4(1, 1, 0, 1))

		local textureWithLightingMaterial = self.resourceManager:getOrCreateMaterial(texWithLightingEffect)
		textureWithLightingMaterial:setPolygonFace(solo.PolygonFace.All)
		textureWithLightingMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		textureWithLightingMaterial:setParameterAutoBinding("invTransposedWorldMatrix", solo.AutoBinding.InverseTransposedWorldMatrix)
		textureWithLightingMaterial:setTextureParameter("mainTex", self.textures.tex2)

		local offscreenCameraRenderedMaterial = self.resourceManager:getOrCreateMaterial(simpleTextureEffect)
		offscreenCameraRenderedMaterial:setPolygonFace(solo.PolygonFace.All)
		offscreenCameraRenderedMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		offscreenCameraRenderedMaterial:setTextureParameter("mainTex", self.textures.offscreenCameraRTT)

		self.materials =
		{
			simpleTexture = simpleTextureMaterial,
			wavySimpleTexture = wavySimpleTextureMaterial,
			red = redMaterial,
			green = greenMaterial,
			blue = blueMaterial,
			white = whiteMaterial,
			checker = checkerMaterial,
			textureWithLighting = textureWithLightingMaterial,
			offscreenCameraRendered = offscreenCameraRenderedMaterial
		}

		self.logger:logInfo("Initialized materials")
	end,

	initMeshes = function(self)
		self.meshes =
		{
			axes = self.resourceManager:getOrLoadMesh("../data/axes.obj"),
			monkey = self.resourceManager:getOrLoadMesh("../data/monkey.obj")
		}
		self.logger:logInfo("Initialized meshes")
	end,

	initRenderTargets = function(self)
		local offscreenRT = self.resourceManager:getOrCreateFrameBuffer("/solo/demo/offscreen-camera-fb")
		offscreenRT:setAttachments({ self.textures.offscreenCameraRTT })

		local mainRT = self.resourceManager:getOrCreateFrameBuffer("/solo/demo/main-camera-fb")
		mainRT:setAttachments({ self.textures.mainCameraRTT })

		self.renderTargets =
		{
			offscreenCameraRT = offscreenRT,
			mainCameraRT = mainRT
		}

		self.logger:logInfo("Initialized render targets")
	end,

	initSkybox = function(self)
		local tex = self.resourceManager:getOrLoadCubeTexture({
			"../data/skyboxes/deep-space/front.png",
			"../data/skyboxes/deep-space/back.png",
			"../data/skyboxes/deep-space/left.png",
			"../data/skyboxes/deep-space/right.png",
			"../data/skyboxes/deep-space/top.png",
			"../data/skyboxes/deep-space/bottom.png"
		}, "/solo/demo/textures/skybox")
		tex:setWrapping(solo.TextureWrapping.Clamp)
		tex:setFiltering(solo.TextureFiltering.Linear)

		local node = self.scene:createNode()
		local renderer = node:addComponent("SkyboxRenderer")
		renderer:setTexture(tex)

		self.logger:logInfo("Initialized skybox")
	end,

	initTexturedQuad = function(self)
		local parent = self.scene:createNode()
		parent:findComponent("Transform"):setLocalPosition(solo.Vector3(5, 0, 0))
		parent:addScript(self.components.createWorldYRotator(self.device))
		self:initAxesMesh(parent)
		local quad = self:createPrefabMeshNode("quad")
		quad:addScript(self.components.createLocalXRotator(self.device));
		quad:findComponent("Transform"):setParent(parent:findComponent("Transform"))
		quad:findComponent("Transform"):setLocalPosition(solo.Vector3(2, 0, 0))
		quad:findComponent("MeshRenderer"):setMaterial(0, self.materials.simpleTexture)
	end,

	initCheckerBox = function(self)
		local node = self:createPrefabMeshNode("cube")
		node:findComponent("MeshRenderer"):setMaterial(0, self.materials.checker)
		node:findComponent("Transform"):setLocalPosition(solo.Vector3(-5, 0, 0))
		node:addScript(self.components.createWorldYRotator(self.device))
	end,

	initMonkey = function(self)
		local node = self.scene:createNode()
		local renderer = node:addComponent("MeshRenderer")
		renderer:setMesh(self.meshes.monkey)
		renderer:setMaterial(0, self.materials.textureWithLighting)
		node:findComponent("Transform"):setLocalPosition(solo.Vector3.zero())
		node:addScript(self.components.createLocalXRotator(self.device))
		return node
	end,

	initWavyMonkey = function(self)
		local node = self.scene:createNode()
		local renderer = node:addComponent("MeshRenderer")
		renderer:setMesh(self.meshes.monkey)
		node:findComponent("Transform"):setLocalPosition(solo.Vector3(0, -3, 0))
		renderer:setMaterial(0, self.materials.wavySimpleTexture)
		node:addScript(self.components.createTimeMaterialUpdater(self.device))
	end,

	initMonitorQuad = function(self, targetNode)
		local canvasSize = self.device:getCanvasSize()

		local parent = self.scene:createNode()
		parent:findComponent("Transform"):setLocalPosition(solo.Vector3(-2, 2, -2))
		parent:addScript(self.components.createWorldYRotator(self.device))
		self:initAxesMesh(parent)

		local quad = self:createPrefabMeshNode("quad")
		local renderer = quad:findComponent("MeshRenderer")
		renderer:setMaterial(0, self.materials.offscreenCameraRendered)
		renderer:getTags():set(self.renderTargetQuadTag)
		local transform = quad:findComponent("Transform")
		transform:setParent(parent:findComponent("Transform"))
		transform:setLocalPosition(solo.Vector3(5, 2, -5))
		transform:setLocalScale(solo.Vector3(5, 5 * canvasSize.y / canvasSize.x, 1))
		quad:addScript(self.components.createTargeter(targetNode:findComponent("Transform")))
	end,

	initObjects = function(self)
		self:initTexturedQuad()
		self:initCheckerBox()
		local monkey = self:initMonkey()
		self:initWavyMonkey()
		self:initMonitorQuad(monkey)
		self.logger:logInfo("Initialized objects")
	end,

	initAxesMesh = function(self, node)
		local renderer = node:addComponent("MeshRenderer")
		renderer:setMesh(self.meshes.axes)
		renderer:setMaterial(0, self.materials.blue)
		renderer:setMaterial(1, self.materials.green)
		renderer:setMaterial(2, self.materials.white)
		renderer:setMaterial(3, self.materials.red)
	end,

	createPrefabMeshNode = function(self, type)
		local mesh = nil
		if type == "quad" then
			mesh = self.resourceManager:getOrCreatePrefabMesh(solo.MeshPrefab.Quad)
		elseif type == "cube" then
			mesh = self.resourceManager:getOrCreatePrefabMesh(solo.MeshPrefab.Cube)
		end

		local node = self.scene:createNode()
		node:addComponent("MeshRenderer"):setMesh(mesh)

		return node
	end,

	initMainCamera = function(self)
		local node = self.scene:createNode()
		local t = node:findComponent("Transform")
		t:setLocalPosition(solo.Vector3(0, 0, 10))
		node:addComponent("Spectator")
		node:addScript(self.components.createEscapeWatcher(self.device))
		node:addScript(self.components.createPostProcessor(self.device, self.textures.mainCameraRTT, self.shaders))
		local cam = node:addComponent("Camera")
		cam:setClearColor(0, 0.6, 0.6, 1)
		cam:setNear(0.05)
		cam:setRenderTarget(self.renderTargets.mainCameraRT)
	end,

	initOffscreenCamera = function(self)
		local canvasSize = self.device:getCanvasSize()
		local node = self.scene:createNode()
		local cam = node:addComponent("Camera")
		cam:setClearColor(1, 0, 1, 1)
		cam:setNear(0.05)
		cam:setRenderTarget(self.renderTargets.offscreenCameraRT)
		cam:setViewport(0, 0, canvasSize.x / 8, canvasSize.y / 8)
		cam:getRenderTags():remove(self.renderTargetQuadTag)
		node:findComponent("Transform"):setLocalPosition(solo.Vector3(0, 0, 10))
	end,

	initCameras = function(self)
		self:initMainCamera()
		self:initOffscreenCamera()
		self.logger:logInfo("Initialized cameras")
	end,

	init = function(self, device)
		self.device = device
		self.resourceManager = device:getResourceManager()
		self.scene = device:getScene()
		self.logger = device:getLogger()
		self:initTextures()
		self:initMaterials()
		self:initRenderTargets()
		self:initMeshes()
		self:initCameras()
		self:initObjects()
		self:initSkybox()
	end
}
