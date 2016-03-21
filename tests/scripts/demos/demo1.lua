function runDemo1()
	local device = solo.device
	local resMgr = device:getResourceManager()
	local scene = device:getScene()
	local logger = device:getLogger()

	local shaders = dofile("../tests/scripts/demos/shaders.lua")
	local createEscapeWatcher = dofile("../tests/scripts/demos/escape-watcher.lua")
	local createRotator = dofile("../tests/scripts/demos/rotator.lua")
	local createTargeter = dofile("../tests/scripts/demos/targeter.lua")
	local createScreenshoter = dofile("../tests/scripts/demos/screenshoter.lua")
	local utils = dofile("../tests/scripts/demos/utils.lua")

	local renderTargetQuadTag = 2

	local canvasSize = device:getCanvasSize()

	local materials
	local meshes
	local offscreenCameraTex

	function createDynamicQuadUpdater(data, mesh, bufferIndex)
		return {
			typeId = 700,

			init = function(self)
				self.time = 0
				self.dir = solo.Vector3.unitZ()
			end,

			update = function(self)
				self.time = self.time + 2 * device:getTimeDelta()
				local offset = 0.3 * math.sin(self.time)
				data[3] = offset
				data[8] = -offset
				data[13] = offset
				data[18] = -offset
				mesh:updateDynamicVertexBuffer(bufferIndex, 0, data, 4)
			end
		}
	end

	function initMaterials()
		local tex1 = utils.loadTexture(resMgr, "../data/freeman.png")
		tex1:setWrapping(solo.TextureWrapping.Clamp)

		local tex2 = utils.loadTexture(resMgr, "../data/cobblestone.png")
		tex2:setWrapping(solo.TextureWrapping.Clamp)

		local tex3 = utils.loadTexture(resMgr, "../data/flammable.png")
		tex3:setWrapping(solo.TextureWrapping.Clamp)

		local simpleTextureEffect = resMgr:getOrCreateEffect(shaders.vertex.basic, shaders.fragment.texture)
		local colorEffect = resMgr:getOrCreateEffect(shaders.vertex.basic, shaders.fragment.color)
		local checkerEffect = resMgr:getOrCreateEffect(shaders.vertex.basic, shaders.fragment.checker)
		local texWithLightingEffect = resMgr:getOrCreateEffect(shaders.vertex.basicLighting, shaders.fragment.textureWithLighting)

		local redMaterial = resMgr:getOrCreateMaterial(colorEffect)
		redMaterial:setPolygonFace(solo.PolygonFace.All)
		redMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		redMaterial:setVector4Parameter("color", solo.Vector4(1, 0, 0, 1))

		local greenMaterial = resMgr:getOrCreateMaterial(colorEffect)
		greenMaterial:setPolygonFace(solo.PolygonFace.All)
		greenMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		greenMaterial:setVector4Parameter("color", solo.Vector4(0, 1, 0, 1))

		local blueMaterial = resMgr:getOrCreateMaterial(colorEffect)
		blueMaterial:setPolygonFace(solo.PolygonFace.All)
		blueMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		blueMaterial:setVector4Parameter("color", solo.Vector4(0, 0, 1, 1))

		local whiteMaterial = resMgr:getOrCreateMaterial(colorEffect)
		whiteMaterial:setPolygonFace(solo.PolygonFace.All)
		whiteMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		whiteMaterial:setVector4Parameter("color", solo.Vector4(1, 1, 1, 1))

		local simpleTextureMaterial = resMgr:getOrCreateMaterial(simpleTextureEffect)
		simpleTextureMaterial:setPolygonFace(solo.PolygonFace.All)
		simpleTextureMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		simpleTextureMaterial:setTextureParameter("mainTex", tex1)

		local transparentTextureMaterial = resMgr:getOrCreateMaterial(simpleTextureEffect)
		transparentTextureMaterial:setPolygonFace(solo.PolygonFace.All)
		transparentTextureMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		transparentTextureMaterial:setTextureParameter("mainTex", tex3)
		transparentTextureMaterial:setTransparent(true)

		local checkerMaterial = resMgr:getOrCreateMaterial(checkerEffect)
		checkerMaterial:setPolygonFace(solo.PolygonFace.All)
		checkerMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix);
		checkerMaterial:setVector4Parameter("color", solo.Vector4(1, 1, 0, 1))

		local textureWithLightingMaterial = resMgr:getOrCreateMaterial(texWithLightingEffect)
		textureWithLightingMaterial:setPolygonFace(solo.PolygonFace.All)
		textureWithLightingMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		textureWithLightingMaterial:setParameterAutoBinding("invTransposedWorldMatrix", solo.AutoBinding.InverseTransposedWorldMatrix)
		textureWithLightingMaterial:setTextureParameter("mainTex", tex2)

		local offscreenCameraRenderedMaterial = resMgr:getOrCreateMaterial(simpleTextureEffect)
		offscreenCameraRenderedMaterial:setPolygonFace(solo.PolygonFace.All)
		offscreenCameraRenderedMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
		offscreenCameraRenderedMaterial:setTextureParameter("mainTex", offscreenCameraTex)

		materials =
		{
			simpleTexture = simpleTextureMaterial,
			transparentTexture = transparentTextureMaterial,
			red = redMaterial,
			green = greenMaterial,
			blue = blueMaterial,
			white = whiteMaterial,
			checker = checkerMaterial,
			textureWithLighting = textureWithLightingMaterial,
			offscreenCameraRendered = offscreenCameraRenderedMaterial
		}

		logger:logInfo("Initialized materials")
	end

	function initMeshes()
		meshes =
		{
			axes = resMgr:getOrLoadMesh("../data/axes.obj"),
			monkey = resMgr:getOrLoadMesh("../data/monkey.obj")
		}
		logger:logInfo("Initialized meshes")
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

	function initTexturedQuad()
		local parent = scene:createNode()
		parent:findComponent("Transform"):setLocalPosition(solo.Vector3(5, 0, 0))
		parent:addScript(createRotator(device, solo.Vector3.unitY(), 1, "world"))
		initAxesMesh(parent)
		local quad = createPrefabMeshNode("quad")
		quad:addScript(createRotator(device, solo.Vector3.unitX(), 1, "local"));
		quad:findComponent("Transform"):setParent(parent:findComponent("Transform"))
		quad:findComponent("Transform"):setLocalPosition(solo.Vector3(2, 0, 0))
		local renderer = quad:findComponent("MeshRenderer")
		renderer:setMaterial(0, materials.transparentTexture)
		renderer:setRenderQueue(3000)
	end

	function initDynamicQuad()
		local layout = solo.VertexBufferLayout()
		layout:add(solo.VertexBufferLayoutSemantics.Position, 3)
		layout:add(solo.VertexBufferLayoutSemantics.TexCoord0, 2)

		local data = {
			-1, -1, 0,	0, 0,
			-1,  1, 0,	0, 1,
		 	1,  1, 0,	1, 1,
			1, -1, 0,	1, 0
		}

		local indices = {
			0, 1, 2,
			0, 2, 3
		}

		local mesh = resMgr:getOrCreateMesh()
		mesh:addDynamicVertexBuffer(layout, data, 4)
		mesh:addPart(indices, 6)

		mesh:setPrimitiveType(solo.PrimitiveType.Triangles)

		local node = scene:createNode()
		node:findComponent("Transform"):setLocalPosition(solo.Vector3(0, 0, -5))

		local renderer = node:addComponent("MeshRenderer")
		renderer:setMesh(mesh)
		renderer:setMaterial(0, materials.simpleTexture)

		node:addScript(createDynamicQuadUpdater(data, mesh, 0))
	end

	function initCheckerBox()
		local node = createPrefabMeshNode("cube")
		node:findComponent("MeshRenderer"):setMaterial(0, materials.checker)
		node:findComponent("Transform"):setLocalPosition(solo.Vector3(-5, 0, 0))
		node:addScript(createRotator(device, solo.Vector3.unitY(), 1, "world"))
	end

	function initMonkey()
		local node = scene:createNode()
		local renderer = node:addComponent("MeshRenderer")
		renderer:setMesh(meshes.monkey)
		renderer:setMaterial(0, materials.textureWithLighting)
		node:findComponent("Transform"):setLocalPosition(solo.Vector3.zero())
		node:addScript(createRotator(device, solo.Vector3.unitX(), 1, "local"))
		return node
	end

	function initMonitorQuad(targetNode)
		local parent = scene:createNode()
		parent:findComponent("Transform"):setLocalPosition(solo.Vector3(-2, 2, -2))
		parent:addScript(createRotator(device, solo.Vector3.unitY(), 1, "world"))
		initAxesMesh(parent)

		local quad = createPrefabMeshNode("quad")
		local renderer = quad:findComponent("MeshRenderer")
		renderer:setMaterial(0, materials.offscreenCameraRendered)
		renderer:getTags():set(renderTargetQuadTag)
		local transform = quad:findComponent("Transform")
		transform:setParent(parent:findComponent("Transform"))
		transform:setLocalPosition(solo.Vector3(5, 2, -5))
		transform:setLocalScale(solo.Vector3(5, 5 * canvasSize.y / canvasSize.x, 1))
		quad:addScript(createTargeter(targetNode:findComponent("Transform")))
	end

	function initAxesMesh(node)
		local renderer = node:addComponent("MeshRenderer")
		renderer:setMesh(meshes.axes)
		renderer:setMaterial(0, materials.blue)
		renderer:setMaterial(1, materials.green)
		renderer:setMaterial(2, materials.white)
		renderer:setMaterial(3, materials.red)
	end

	function createPrefabMeshNode(type)
		local mesh = nil
		if type == "quad" then
			mesh = resMgr:getOrCreatePrefabMesh(solo.MeshPrefab.Quad)
		elseif type == "cube" then
			mesh = resMgr:getOrCreatePrefabMesh(solo.MeshPrefab.Cube)
		end

		local node = scene:createNode()
		node:addComponent("MeshRenderer"):setMesh(mesh)

		return node
	end

	function initMainCamera()
		local node = scene:createNode()
		local t = node:findComponent("Transform")
		t:setLocalPosition(solo.Vector3(0, 5, 10))
		t:lookAt(solo.Vector3.zero(), solo.Vector3.unitY())
		node:addComponent("Spectator")
		node:addScript(createEscapeWatcher(device))
		node:addScript(createScreenshoter(device, "demo1-screenshot.bmp"))
		local cam = node:addComponent("Camera")
		cam:setClearColor(0, 0.6, 0.6, 1)
		cam:setNear(0.05)
	end

	function initOffscreenCamera()
		offscreenCameraTex = resMgr:getOrCreateTexture2D()
		offscreenCameraTex:setData(solo.ColorFormat.RGB, {}, math.floor(canvasSize.x / 8), math.floor(canvasSize.y / 8))
		offscreenCameraTex:setFiltering(solo.TextureFiltering.Nearest)
		offscreenCameraTex:setWrapping(solo.TextureWrapping.Clamp)

		local fb = resMgr:getOrCreateFrameBuffer()
		fb:setAttachments({ offscreenCameraTex })

		local node = scene:createNode()
		local cam = node:addComponent("Camera")
		cam:setClearColor(1, 0, 1, 1)
		cam:setNear(0.05)
		cam:setRenderTarget(fb)
		cam:setViewport(0, 0, canvasSize.x / 8, canvasSize.y / 8)
		cam:getRenderTags():remove(renderTargetQuadTag)
		node:findComponent("Transform"):setLocalPosition(solo.Vector3(0, 0, 10))
	end

	function initCameras()
		initMainCamera()
		initOffscreenCamera()
		logger:logInfo("Initialized cameras")
	end

	function initObjects()
		initTexturedQuad()
		initCheckerBox()
		local monkey = initMonkey()
		initMonitorQuad(monkey)
		initDynamicQuad()
		logger:logInfo("Initialized objects")
	end

	initCameras()
	initMaterials()
	initMeshes()
	initObjects()
	initSkybox()

	device:run()
end

runDemo1()
