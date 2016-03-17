return {
	shaders = dofile("../tests/scripts/demos/shaders.lua"),
	createEscapeWatcher = dofile("../tests/scripts/demos/escape-watcher.lua"),
	createRotator = dofile("../tests/scripts/demos/rotator.lua"),
	createTargeter = dofile("../tests/scripts/demos/targeter.lua"),
	utils = dofile("../tests/scripts/demos/utils.lua"),

	renderTargetQuadTag = 2,

	initMaterials = function(self)
		local tex1 = self.utils.loadTexture(self.resMgr, "../data/freeman1.png")
		local tex2 = self.utils.loadTexture(self.resMgr, "../data/cobblestone.png")

		local simpleTextureEffect = self.resMgr:getOrCreateEffect(self.shaders.vertex.basic, self.shaders.fragment.texture)
		local colorEffect = self.resMgr:getOrCreateEffect(self.shaders.vertex.basic, self.shaders.fragment.color)
		local checkerEffect = self.resMgr:getOrCreateEffect(self.shaders.vertex.basic, self.shaders.fragment.checker)
		local texWithLightingEffect = self.resMgr:getOrCreateEffect(self.shaders.vertex.basicLighting, self.shaders.fragment.textureWithLighting)

		local redMaterial = self.resMgr:getOrCreateMaterial(colorEffect)
		redMaterial:setPolygonFace(solo.PolygonFace.All)
		redMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		redMaterial:setVector4Parameter("color", solo.Vector4(1, 0, 0, 1))

		local greenMaterial = self.resMgr:getOrCreateMaterial(colorEffect)
		greenMaterial:setPolygonFace(solo.PolygonFace.All)
		greenMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		greenMaterial:setVector4Parameter("color", solo.Vector4(0, 1, 0, 1))

		local blueMaterial = self.resMgr:getOrCreateMaterial(colorEffect)
		blueMaterial:setPolygonFace(solo.PolygonFace.All)
		blueMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		blueMaterial:setVector4Parameter("color", solo.Vector4(0, 0, 1, 1))

		local whiteMaterial = self.resMgr:getOrCreateMaterial(colorEffect)
		whiteMaterial:setPolygonFace(solo.PolygonFace.All)
		whiteMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		whiteMaterial:setVector4Parameter("color", solo.Vector4(1, 1, 1, 1))

		local simpleTextureMaterial = self.resMgr:getOrCreateMaterial(simpleTextureEffect)
		simpleTextureMaterial:setPolygonFace(solo.PolygonFace.All)
		simpleTextureMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		simpleTextureMaterial:setTextureParameter("mainTex", tex1)

		local checkerMaterial = self.resMgr:getOrCreateMaterial(checkerEffect)
		checkerMaterial:setPolygonFace(solo.PolygonFace.All)
		checkerMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix);
		checkerMaterial:setVector4Parameter("color", solo.Vector4(1, 1, 0, 1))

		local textureWithLightingMaterial = self.resMgr:getOrCreateMaterial(texWithLightingEffect)
		textureWithLightingMaterial:setPolygonFace(solo.PolygonFace.All)
		textureWithLightingMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		textureWithLightingMaterial:setParameterAutoBinding("invTransposedWorldMatrix", solo.AutoBinding.InverseTransposedWorldMatrix)
		textureWithLightingMaterial:setTextureParameter("mainTex", tex2)

		local offscreenCameraRenderedMaterial = self.resMgr:getOrCreateMaterial(simpleTextureEffect)
		offscreenCameraRenderedMaterial:setPolygonFace(solo.PolygonFace.All)
		offscreenCameraRenderedMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		offscreenCameraRenderedMaterial:setTextureParameter("mainTex", self.offscreenCameraTex)

		self.materials =
		{
			simpleTexture = simpleTextureMaterial,
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
			axes = self.resMgr:getOrLoadMesh("../data/axes.obj"),
			monkey = self.resMgr:getOrLoadMesh("../data/monkey.obj")
		}
		self.logger:logInfo("Initialized meshes")
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

	initTexturedQuad = function(self)
		local parent = self.scene:createNode()
		parent:findComponent("Transform"):setLocalPosition(solo.Vector3(5, 0, 0))
		parent:addScript(self.createRotator(self.device, solo.Vector3.unitY(), 1, "world"))
		self:initAxesMesh(parent)
		local quad = self:createPrefabMeshNode("quad")
		quad:addScript(self.createRotator(self.device, solo.Vector3.unitX(), 1, "local"));
		quad:findComponent("Transform"):setParent(parent:findComponent("Transform"))
		quad:findComponent("Transform"):setLocalPosition(solo.Vector3(2, 0, 0))
		quad:findComponent("MeshRenderer"):setMaterial(0, self.materials.simpleTexture)
	end,

	initCheckerBox = function(self)
		local node = self:createPrefabMeshNode("cube")
		node:findComponent("MeshRenderer"):setMaterial(0, self.materials.checker)
		node:findComponent("Transform"):setLocalPosition(solo.Vector3(-5, 0, 0))
		node:addScript(self.createRotator(self.device, solo.Vector3.unitY(), 1, "world"))
	end,

	initMonkey = function(self)
		local node = self.scene:createNode()
		local renderer = node:addComponent("MeshRenderer")
		renderer:setMesh(self.meshes.monkey)
		renderer:setMaterial(0, self.materials.textureWithLighting)
		node:findComponent("Transform"):setLocalPosition(solo.Vector3.zero())
		node:addScript(self.createRotator(self.device, solo.Vector3.unitX(), 1, "local"))
		return node
	end,

	initMonitorQuad = function(self, targetNode)
		local canvasSize = self.device:getCanvasSize()

		local parent = self.scene:createNode()
		parent:findComponent("Transform"):setLocalPosition(solo.Vector3(-2, 2, -2))
		parent:addScript(self.createRotator(self.device, solo.Vector3.unitY(), 1, "world"))
		self:initAxesMesh(parent)

		local quad = self:createPrefabMeshNode("quad")
		local renderer = quad:findComponent("MeshRenderer")
		renderer:setMaterial(0, self.materials.offscreenCameraRendered)
		renderer:getTags():set(self.renderTargetQuadTag)
		local transform = quad:findComponent("Transform")
		transform:setParent(parent:findComponent("Transform"))
		transform:setLocalPosition(solo.Vector3(5, 2, -5))
		transform:setLocalScale(solo.Vector3(5, 5 * canvasSize.y / canvasSize.x, 1))
		quad:addScript(self.createTargeter(targetNode:findComponent("Transform")))
	end,

	initObjects = function(self)
		self:initTexturedQuad()
		self:initCheckerBox()
		local monkey = self:initMonkey()
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
			mesh = self.resMgr:getOrCreatePrefabMesh(solo.MeshPrefab.Quad)
		elseif type == "cube" then
			mesh = self.resMgr:getOrCreatePrefabMesh(solo.MeshPrefab.Cube)
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
		node:addScript(self.createEscapeWatcher(self.device))
		local cam = node:addComponent("Camera")
		cam:setClearColor(0, 0.6, 0.6, 1)
		cam:setNear(0.05)
	end,

	initOffscreenCamera = function(self)
		local canvasSize = self.device:getCanvasSize()

		self.offscreenCameraTex = self.resMgr:getOrCreateTexture2D()
		self.offscreenCameraTex:setData(solo.ColorFormat.RGB, {}, math.floor(canvasSize.x / 8), math.floor(canvasSize.y / 8))
		self.offscreenCameraTex:setFiltering(solo.TextureFiltering.Nearest)
		self.offscreenCameraTex:setWrapping(solo.TextureWrapping.Clamp)

		local fb = self.resMgr:getOrCreateFrameBuffer()
		fb:setAttachments({ self.offscreenCameraTex })

		local node = self.scene:createNode()
		local cam = node:addComponent("Camera")
		cam:setClearColor(1, 0, 1, 1)
		cam:setNear(0.05)
		cam:setRenderTarget(fb)
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
		self.resMgr = device:getResourceManager()
		self.scene = device:getScene()
		self.logger = device:getLogger()
		self:initCameras()
		self:initMaterials()
		self:initMeshes()
		self:initObjects()
		self:initSkybox()
	end
}
