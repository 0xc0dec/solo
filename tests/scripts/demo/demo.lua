local shaders = dofile("../tests/scripts/demo/demo.shaders.lua")
dofile("../tests/scripts/demo/demo.components.lua")

local RENDER_TARGET_QUAD_TAG = 2

local demo = {}


function loadTexture(path)
	local texture = resourceManager:getOrLoadTexture2D(path)
	texture:generateMipmaps()
	texture:setFiltering(solo.TextureFiltering.Linear)
	texture:setAnisotropyLevel(8)
	return texture
end


function initTextures()
	local canvasSize = device:getCanvasSize()

	local mainCameraRTT = resourceManager:getOrCreateTexture2D("demo/main-camera-rtt")
	mainCameraRTT:setData(solo.ColorFormat.RGB, {}, canvasSize.x, canvasSize.y)
	mainCameraRTT:setFiltering(solo.TextureFiltering.Nearest)
	mainCameraRTT:setWrapping(solo.TextureWrapping.Clamp)

	local offscreenCameraRTT = resourceManager:getOrCreateTexture2D("demo/offscreen-camera-rtt")
	offscreenCameraRTT:setData(solo.ColorFormat.RGB, {}, canvasSize.x / 8, canvasSize.y / 8)
	offscreenCameraRTT:setFiltering(solo.TextureFiltering.Nearest)
	offscreenCameraRTT:setWrapping(solo.TextureWrapping.Clamp)

	demo.textures =
	{
		tex1 = loadTexture("../data/freeman1.png"),
		tex2 = loadTexture("../data/freeman2.png"),
		mainCameraRTT = mainCameraRTT,
		offscreenCameraRTT = offscreenCameraRTT,
	}
end


function initEffects()
	local simpleTextureEffect = resourceManager:getOrCreateEffect(shaders.vsBasic, shaders.fsTexture)
	demo.effects =
	{
		simpleTextureEffect = simpleTextureEffect
	}
end


function initMaterials()
	local colorEffect = resourceManager:getOrCreateEffect(shaders.vsBasic, shaders.fsColor)
	local redMaterial = resourceManager:getOrCreateMaterial(colorEffect)
	redMaterial:setPolygonFace(solo.PolygonFace.All)
	redMaterial:getParameter("worldViewProjMatrix"):bindValue(solo.AutoBinding.WorldViewProjectionMatrix)
	redMaterial:getParameter("color"):setVector4(solo.Vector4(1, 0, 0, 1))

	local greenMaterial = resourceManager:getOrCreateMaterial(colorEffect)
	greenMaterial:setPolygonFace(solo.PolygonFace.All)
	greenMaterial:getParameter("worldViewProjMatrix"):bindValue(solo.AutoBinding.WorldViewProjectionMatrix)
	greenMaterial:getParameter("color"):setVector4(solo.Vector4(0, 1, 0, 1))

	local blueMaterial = resourceManager:getOrCreateMaterial(colorEffect)
	blueMaterial:setPolygonFace(solo.PolygonFace.All)
	blueMaterial:getParameter("worldViewProjMatrix"):bindValue(solo.AutoBinding.WorldViewProjectionMatrix)
	blueMaterial:getParameter("color"):setVector4(solo.Vector4(0, 0, 1, 1))

	local whiteMaterial = resourceManager:getOrCreateMaterial(colorEffect)
	whiteMaterial:setPolygonFace(solo.PolygonFace.All)
	whiteMaterial:getParameter("worldViewProjMatrix"):bindValue(solo.AutoBinding.WorldViewProjectionMatrix)
	whiteMaterial:getParameter("color"):setVector4(solo.Vector4(1, 1, 1, 1))

	local simpleTexture = resourceManager:getOrCreateMaterial(demo.effects.simpleTextureEffect)
	simpleTexture:setPolygonFace(solo.PolygonFace.All)
	simpleTexture:getParameter("worldViewProjMatrix"):bindValue(solo.AutoBinding.WorldViewProjectionMatrix)
	simpleTexture:getParameter("mainTex"):setTexture(demo.textures.tex1)

	local checkerEffect = resourceManager:getOrCreateEffect(shaders.vsBasic, shaders.fsChecker)
	local checkerMaterial = resourceManager:getOrCreateMaterial(checkerEffect)
	checkerMaterial:setPolygonFace(solo.PolygonFace.All)
	checkerMaterial:getParameter("worldViewProjMatrix"):bindValue(solo.AutoBinding.WorldViewProjectionMatrix);
	checkerMaterial:getParameter("color"):setVector4(solo.Vector4(1, 1, 0, 1))

	local texWithLightingEffect = resourceManager:getOrCreateEffect(shaders.vsBasicLighting, shaders.fsTextureWithLighting)
	local textureWithLightingMaterial = resourceManager:getOrCreateMaterial(texWithLightingEffect)
	textureWithLightingMaterial:setPolygonFace(solo.PolygonFace.All)
	textureWithLightingMaterial:getParameter("worldViewProjMatrix"):bindValue(solo.AutoBinding.WorldViewProjectionMatrix)
	textureWithLightingMaterial:getParameter("normalMatrix"):bindValue(solo.AutoBinding.InverseTransposedWorldMatrix)
	textureWithLightingMaterial:getParameter("mainTex"):setTexture(demo.textures.tex2)

	local offscreenCameraRenderedMaterial = resourceManager:getOrCreateMaterial(demo.effects.simpleTextureEffect)
	offscreenCameraRenderedMaterial:setPolygonFace(solo.PolygonFace.All)
	offscreenCameraRenderedMaterial:getParameter("worldViewProjMatrix"):bindValue(solo.AutoBinding.WorldViewProjectionMatrix)
	offscreenCameraRenderedMaterial:getParameter("mainTex"):setTexture(demo.textures.offscreenCameraRTT)

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
end


function initModels()
	demo.models =
	{
		axes = resourceManager:getOrLoadModel("../data/axes.obj"),
		monkey = resourceManager:getOrLoadModel("../data/monkey_hires.obj")
	}
end


function initRenderTargets()
	local offscreenCameraRT = resourceManager:getOrCreateRenderTarget("demo/offscreen-camera-rt")
	offscreenCameraRT:setColorAttachment(0, demo.textures.offscreenCameraRTT)

	local mainCameraRT = resourceManager:getOrCreateRenderTarget("demo/main-camera-rt")
	mainCameraRT:setColorAttachment(0, demo.textures.mainCameraRTT)

	demo.renderTargets =
	{
		offscreenCameraRT = offscreenCameraRT,
		mainCameraRT = mainCameraRT
	}
end


function initSkybox()
	local texSkybox = resourceManager:getOrLoadCubeTexture({
		"../data/skyboxes/deep-space/front.png",
		"../data/skyboxes/deep-space/back.png",
		"../data/skyboxes/deep-space/left.png",
		"../data/skyboxes/deep-space/right.png",
		"../data/skyboxes/deep-space/top.png",
		"../data/skyboxes/deep-space/bottom.png"
	}, "demo/textures/skybox")
	texSkybox:setWrapping(solo.TextureWrapping.Clamp)

	local skybox = scene:createNode()
	local skyboxRenderer = skybox:addComponent("SkyboxRenderer")
	skyboxRenderer:setTexture(texSkybox)
end


function initObjects()
	local canvasSize = device:getCanvasSize()

	-- Textured quad
	parent = scene:createNode()
	parent:findComponent("Transform"):setLocalPosition(solo.Vector3(5, 0, 0))
	parent:addScript(createWorldYRotator())
	-- initAxesModel(parent)
	quad = createQuad()
	quad:addScript(createLocalXRotator());
	quad:findComponent("Transform"):setParent(parent:findComponent("Transform"))
	quad:findComponent("Transform"):setLocalPosition(solo.Vector3(2, 0, 0))
	quad:findComponent("MeshRenderer"):setMaterial(0, demo.materials.simpleTexture)

	-- Box
	node = createQuad()
	rebuildToBoxMesh(node)
	node:findComponent("MeshRenderer"):setMaterial(0, demo.materials.checker)
	node:findComponent("Transform"):setLocalPosition(solo.Vector3(-5, 0, 0))
	node:addScript(createWorldYRotator())

	-- Monkey
	-- local node = scene:createNode()
	-- local renderer = node:addComponent("MeshRenderer")
	-- renderer:setModel(demo.models.monkey)
	-- renderer:setMaterial(demo.materials.textureWithLighting)
	-- node:findComponent("Transform"):setLocalPosition(solo.Vector3.zero())
	-- node:addScript(createLocalXRotator())

	-- RTT quad
	local parent = scene:createNode()
	parent:findComponent("Transform"):setLocalPosition(solo.Vector3(-2, 2, -2))
	parent:addScript(createWorldYRotator())
	-- initAxesModel(parent)

	local quad = createQuad()
	local renderer = quad:findComponent("MeshRenderer")
	renderer:setMaterial(0, demo.materials.offscreenCameraRendered)
	renderer:getTags():set(RENDER_TARGET_QUAD_TAG)
	local quadTransform = quad:findComponent("Transform")
	quadTransform:setParent(parent:findComponent("Transform"))
	quadTransform:setLocalPosition(solo.Vector3(5, 2, -5))
	quadTransform:setLocalScale(solo.Vector3(5, 5 * canvasSize.y / canvasSize.x, 1))
	-- quad:addScript(createTargeter(node:findComponent("Transform"))) -- monkey
end


function rebuildToBoxMesh(node)
	local mesh = node:findComponent("MeshRenderer"):getMesh()
	mesh:rebuildAsBox()
end


function initAxesModel(node)
	local renderer = node:addComponent("MeshRenderer")
	renderer:setModel(demo.models.axes)
	renderer:setMaterial(0, demo.materials.blue)
	renderer:setMaterial(1, demo.materials.green)
	renderer:setMaterial(2, demo.materials.white)
	renderer:setMaterial(3, demo.materials.red)
end


function createQuad()
	local mesh = resourceManager:getOrCreateMesh()
	mesh:rebuildAsQuad()

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
end


function init()
	initTextures()
	initEffects()
	initMaterials()
	initRenderTargets()
	-- initModels()
	initCameras()
	-- initObjects()
	initSkybox()
	print("Initialized")
end


device = solo.Device.create(solo.DeviceCreationArgs(solo.DeviceMode.OpenGL, 800, 600, false))
device:setStartCallback(function()
	scene = device:getScene()
	resourceManager = device:getResourceManager()
	local _, err = pcall(init)
	if err then
		print(err)
	end
end)
device:run()
