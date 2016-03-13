local shaders = dofile("../tests/scripts/demo/demo.shaders.lua")
dofile("../tests/scripts/demo/demo.components.lua")

local RENDER_TARGET_QUAD_TAG = 2

local demo = {}


function loadTexture(path)
	local texture = resourceManager:getOrLoadTexture2D(path)
	texture:generateMipmaps()
	texture:setFiltering(solo.TextureFiltering.LinearMipmapNearest)
	texture:setAnisotropyLevel(8)
	return texture
end


function initTextures()
	local canvasSize = device:getCanvasSize()

	local mainCameraRTT = resourceManager:getOrCreateTexture2D("/solo/demo/main-camera-rtt")
	mainCameraRTT:setData(solo.ColorFormat.RGB, {}, canvasSize.x, canvasSize.y)
	mainCameraRTT:setFiltering(solo.TextureFiltering.Nearest)
	mainCameraRTT:setWrapping(solo.TextureWrapping.Clamp)

	local offscreenCameraRTT = resourceManager:getOrCreateTexture2D("/solo/demo/offscreen-camera-rtt")
	offscreenCameraRTT:setData(solo.ColorFormat.RGB, {}, math.floor(canvasSize.x / 8), math.floor(canvasSize.y / 8))
	offscreenCameraRTT:setFiltering(solo.TextureFiltering.Nearest)
	offscreenCameraRTT:setWrapping(solo.TextureWrapping.Clamp)

	demo.textures =
	{
		tex1 = loadTexture("../data/freeman1.png"),
		tex2 = loadTexture("../data/cobblestone.png"),
		mainCameraRTT = mainCameraRTT,
		offscreenCameraRTT = offscreenCameraRTT,
	}

	logger:logInfo("Initialized textures")
end


function initEffects()
	local simpleTextureEffect = resourceManager:getOrCreateEffect(shaders.vsBasic, shaders.fsTexture)
	demo.effects =
	{
		simpleTextureEffect = simpleTextureEffect
	}

	logger:logInfo("Initialized effects")
end


function initMaterials()
	local colorEffect = resourceManager:getOrCreateEffect(shaders.vsBasic, shaders.fsColor)
	local redMaterial = resourceManager:getOrCreateMaterial(colorEffect)
	redMaterial:setPolygonFace(solo.PolygonFace.All)
	redMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
	redMaterial:setVector4Parameter("color", solo.Vector4(1, 0, 0, 1))

	local greenMaterial = resourceManager:getOrCreateMaterial(colorEffect)
	greenMaterial:setPolygonFace(solo.PolygonFace.All)
	greenMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
	greenMaterial:setVector4Parameter("color", solo.Vector4(0, 1, 0, 1))

	local blueMaterial = resourceManager:getOrCreateMaterial(colorEffect)
	blueMaterial:setPolygonFace(solo.PolygonFace.All)
	blueMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
	blueMaterial:setVector4Parameter("color", solo.Vector4(0, 0, 1, 1))

	local whiteMaterial = resourceManager:getOrCreateMaterial(colorEffect)
	whiteMaterial:setPolygonFace(solo.PolygonFace.All)
	whiteMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
	whiteMaterial:setVector4Parameter("color", solo.Vector4(1, 1, 1, 1))

	local simpleTexture = resourceManager:getOrCreateMaterial(demo.effects.simpleTextureEffect)
	simpleTexture:setPolygonFace(solo.PolygonFace.All)
	simpleTexture:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
	simpleTexture:setTextureParameter("mainTex", demo.textures.tex1)

	local checkerEffect = resourceManager:getOrCreateEffect(shaders.vsBasic, shaders.fsChecker)
	local checkerMaterial = resourceManager:getOrCreateMaterial(checkerEffect)
	checkerMaterial:setPolygonFace(solo.PolygonFace.All)
	checkerMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix);
	checkerMaterial:setVector4Parameter("color", solo.Vector4(1, 1, 0, 1))

	local texWithLightingEffect = resourceManager:getOrCreateEffect(shaders.vsBasicLighting, shaders.fsTextureWithLighting)
	local textureWithLightingMaterial = resourceManager:getOrCreateMaterial(texWithLightingEffect)
	textureWithLightingMaterial:setPolygonFace(solo.PolygonFace.All)
	textureWithLightingMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
	textureWithLightingMaterial:setParameterAutoBinding("normalMatrix", solo.AutoBinding.InverseTransposedWorldMatrix)
	textureWithLightingMaterial:setTextureParameter("mainTex", demo.textures.tex2)

	local offscreenCameraRenderedMaterial = resourceManager:getOrCreateMaterial(demo.effects.simpleTextureEffect)
	offscreenCameraRenderedMaterial:setPolygonFace(solo.PolygonFace.All)
	offscreenCameraRenderedMaterial:setParameterAutoBinding("worldViewProjMatrix", solo.AutoBinding.WorldViewProjectionMatrix)
	offscreenCameraRenderedMaterial:setTextureParameter("mainTex", demo.textures.offscreenCameraRTT)

	demo.materials =
	{
		simpleTexture = simpleTexture,
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
	demo.meshes =
	{
		axes = resourceManager:getOrLoadMesh("../data/axes.obj"),
		monkey = resourceManager:getOrLoadMesh("../data/monkey.obj")
	}
	logger:logInfo("Initialized meshes")
end


function initRenderTargets()
	local offscreenCameraRT = resourceManager:getOrCreateFrameBuffer("/solo/demo/offscreen-camera-fb")
	offscreenCameraRT:setAttachments({ demo.textures.offscreenCameraRTT })

	local mainCameraRT = resourceManager:getOrCreateFrameBuffer("/solo/demo/main-camera-fb")
	mainCameraRT:setAttachments({ demo.textures.mainCameraRTT })

	demo.renderTargets =
	{
		offscreenCameraRT = offscreenCameraRT,
		mainCameraRT = mainCameraRT
	}

	logger:logInfo("Initialized render targets")
end


function initSkybox()
	local texSkybox = resourceManager:getOrLoadCubeTexture({
		"../data/skyboxes/deep-space/front.png",
		"../data/skyboxes/deep-space/back.png",
		"../data/skyboxes/deep-space/left.png",
		"../data/skyboxes/deep-space/right.png",
		"../data/skyboxes/deep-space/top.png",
		"../data/skyboxes/deep-space/bottom.png"
	}, "/solo/demo/textures/skybox")
	texSkybox:setWrapping(solo.TextureWrapping.Clamp)

	local skybox = scene:createNode()
	local skyboxRenderer = skybox:addComponent("SkyboxRenderer")
	skyboxRenderer:setTexture(texSkybox)

	logger:logInfo("Initialized skybox")
end


function initObjects()
	local canvasSize = device:getCanvasSize()

	-- Textured quad
	parent = scene:createNode()
	parent:findComponent("Transform"):setLocalPosition(solo.Vector3(5, 0, 0))
	parent:addScript(createWorldYRotator())
	initAxesMesh(parent)
	quad = createPrefabMeshNode("quad")
	quad:addScript(createLocalXRotator());
	quad:findComponent("Transform"):setParent(parent:findComponent("Transform"))
	quad:findComponent("Transform"):setLocalPosition(solo.Vector3(2, 0, 0))
	quad:findComponent("MeshRenderer"):setMaterial(0, demo.materials.simpleTexture)

	-- Box
	node = createPrefabMeshNode("cube")
	node:findComponent("MeshRenderer"):setMaterial(0, demo.materials.checker)
	node:findComponent("Transform"):setLocalPosition(solo.Vector3(-5, 0, 0))
	node:addScript(createWorldYRotator())

	-- Monkey
	local node = scene:createNode()
	local renderer = node:addComponent("MeshRenderer")
	renderer:setMesh(demo.meshes.monkey)
	renderer:setMaterial(0, demo.materials.textureWithLighting)
	node:findComponent("Transform"):setLocalPosition(solo.Vector3.zero())
	node:addScript(createLocalXRotator())

	-- RTT quad
	local parent = scene:createNode()
	parent:findComponent("Transform"):setLocalPosition(solo.Vector3(-2, 2, -2))
	parent:addScript(createWorldYRotator())
	initAxesMesh(parent)

	local quad = createPrefabMeshNode("quad")
	local renderer = quad:findComponent("MeshRenderer")
	renderer:setMaterial(0, demo.materials.offscreenCameraRendered)
	renderer:getTags():set(RENDER_TARGET_QUAD_TAG)
	local quadTransform = quad:findComponent("Transform")
	quadTransform:setParent(parent:findComponent("Transform"))
	quadTransform:setLocalPosition(solo.Vector3(5, 2, -5))
	quadTransform:setLocalScale(solo.Vector3(5, 5 * canvasSize.y / canvasSize.x, 1))
	quad:addScript(createTargeter(node:findComponent("Transform"))) -- monkey

	logger:logInfo("Initialized objects")
end


function initAxesMesh(node)
	local renderer = node:addComponent("MeshRenderer")
	renderer:setMesh(demo.meshes.axes)
	renderer:setMaterial(0, demo.materials.blue)
	renderer:setMaterial(1, demo.materials.green)
	renderer:setMaterial(2, demo.materials.white)
	renderer:setMaterial(3, demo.materials.red)
end


function createPrefabMeshNode(type)
	local mesh = nil
	if type == "quad" then
		mesh = resourceManager:getOrCreatePrefabMesh(solo.MeshPrefab.Quad)
	elseif type == "cube" then
		mesh = resourceManager:getOrCreatePrefabMesh(solo.MeshPrefab.Cube)
	end

	local node = scene:createNode()
	node:addComponent("MeshRenderer"):setMesh(mesh)

	return node
end


function initCameras()
	local mainCameraNode = scene:createNode()
	local mainCameraTransform = mainCameraNode:findComponent("Transform")
	mainCameraTransform:setLocalPosition(solo.Vector3(0, 2, 15))
	mainCameraNode:addComponent("Spectator")
	mainCameraNode:addScript(createEscapeWatcher())
	local mainCamera = mainCameraNode:addComponent("Camera")
	mainCamera:setClearColor(0, 0.6, 0.6, 1)
	mainCamera:setNear(0.05)
	mainCamera:setRenderTarget(demo.renderTargets.mainCameraRT)
	mainCameraNode:addScript(createPostProcessor(demo.textures.mainCameraRTT, shaders))

	local canvasSize = device:getCanvasSize()
	local offscreenCameraNode = scene:createNode()
	local offscreenCamera = offscreenCameraNode:addComponent("Camera")
	offscreenCamera:setClearColor(1, 0, 1, 1)
	offscreenCamera:setNear(0.05)
	offscreenCamera:setRenderTarget(demo.renderTargets.offscreenCameraRT)
	offscreenCamera:setViewport(0, 0, canvasSize.x / 8, canvasSize.y / 8)
	offscreenCamera:getRenderTags():remove(RENDER_TARGET_QUAD_TAG)
	offscreenCameraNode:findComponent("Transform"):setLocalPosition(solo.Vector3(0, 0, 10))

	logger:logInfo("Initialized cameras")
end


function init()
	initTextures()
	initEffects()
	initMaterials()
	initRenderTargets()
	initMeshes()
	initCameras()
	initObjects()
	initSkybox()
end


local args = solo.DeviceCreationArgs(solo.DeviceMode.OpenGL, 1366, 700, false, "Solo Tester")
args.logFilePath = "demo.log"

device = solo.Device.create(args)
logger = device:getLogger()

device:setStartCallback(function()
	scene = device:getScene()
	resourceManager = device:getResourceManager()
	local _, err = pcall(init)
	if err then
		logger:logCritical(err)
	end
end)
device:run()
