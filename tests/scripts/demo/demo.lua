local shaders = dofile("../tests/scripts/demo/demo.shaders.lua")
dofile("../tests/scripts/demo/demo.components.lua")

function loadTexture(path)
	local texture = resourceManager:getOrLoadTexture(path)
	texture:generateMipmaps()
	texture:setFilterMode(solo.TextureFilter_Linear, solo.TextureFilter_Linear)
	texture:setAnisotropyLevel(8)
	return texture
end


function initMaterials()
	local tex1 = loadTexture("../data/freeman1.png")
	local tex2 = loadTexture("../data/freeman2.png")

	local result = {}

	local colorEffect = resourceManager:getOrCreateEffect(shaders.vsBasic, shaders.fsColor)
	local redMaterial = resourceManager:getOrCreateMaterial(colorEffect)
	redMaterial:setPolygonFace(solo.PolygonFace_All)
	redMaterial:getParameter("worldViewProjMatrix"):bindValue(solo.AutoBinding_WorldViewProjectionMatrix)
	redMaterial:getParameter("color"):setVector4(solo.Vector4(1, 0, 0, 1))

	local greenMaterial = resourceManager:getOrCreateMaterial(colorEffect)
	greenMaterial:setPolygonFace(solo.PolygonFace_All)
	greenMaterial:getParameter("worldViewProjMatrix"):bindValue(solo.AutoBinding_WorldViewProjectionMatrix)
	greenMaterial:getParameter("color"):setVector4(solo.Vector4(0, 1, 0, 1))

	local blueMaterial = resourceManager:getOrCreateMaterial(colorEffect)
	blueMaterial:setPolygonFace(solo.PolygonFace_All)
	blueMaterial:getParameter("worldViewProjMatrix"):bindValue(solo.AutoBinding_WorldViewProjectionMatrix)
	blueMaterial:getParameter("color"):setVector4(solo.Vector4(0, 0, 1, 1))

	local whiteMaterial = resourceManager:getOrCreateMaterial(colorEffect)
	whiteMaterial:setPolygonFace(solo.PolygonFace_All)
	whiteMaterial:getParameter("worldViewProjMatrix"):bindValue(solo.AutoBinding_WorldViewProjectionMatrix)
	whiteMaterial:getParameter("color"):setVector4(solo.Vector4(1, 1, 1, 1))

	local texEffect = resourceManager:getOrCreateEffect(shaders.vsBasic, shaders.fsTexture)
	local texMaterial = resourceManager:getOrCreateMaterial(texEffect)
	texMaterial:setPolygonFace(solo.PolygonFace_All)
	texMaterial:getParameter("worldViewProjMatrix"):bindValue(solo.AutoBinding_WorldViewProjectionMatrix)
	texMaterial:getParameter("mainTex"):setTexture(tex1)

	local checkerEffect = resourceManager:getOrCreateEffect(shaders.vsBasic, shaders.fsChecker)
	local checkerMaterial = resourceManager:getOrCreateMaterial(checkerEffect)
	checkerMaterial:setPolygonFace(solo.PolygonFace_All)
	checkerMaterial:getParameter("worldViewProjMatrix"):bindValue(solo.AutoBinding_WorldViewProjectionMatrix);
	checkerMaterial:getParameter("color"):setVector4(solo.Vector4(1, 1, 0, 1))

	local texWithLightingEffect = resourceManager:getOrCreateEffect(shaders.vsBasicLighting, shaders.fsTextureWithLighting)
	local texWithLightingMaterial = resourceManager:getOrCreateMaterial(texWithLightingEffect)
	texWithLightingMaterial:setPolygonFace(solo.PolygonFace_All)
	texWithLightingMaterial:getParameter("worldViewProjMatrix"):bindValue(solo.AutoBinding_WorldViewProjectionMatrix)
	texWithLightingMaterial:getParameter("normalMatrix"):bindValue(solo.AutoBinding_InverseTransposedWorldMatrix)
	texWithLightingMaterial:getParameter("mainTex"):setTexture(tex2)

	return
	{
		texEffect = texEffect,
		red = redMaterial,
		green = greenMaterial,
		blue = blueMaterial,
		white = whiteMaterial,
		tex = texMaterial,
		checker = checkerMaterial,
		texWithLighting = texWithLightingMaterial
	}
end


function initModels()
	return
	{
		axes = resourceManager:getOrLoadModel("../data/axes.obj"),
		monkey = resourceManager:getOrLoadModel("../data/monkey_hires.obj")
	}
end


function initRenderTarget(materials)
	local canvasSize = device:getCanvasSize()
	local renderTexture = resourceManager:getOrCreateTexture2D("RTT")
	renderTexture:setData(solo.ColorFormat_RGB, {}, canvasSize.x / 8, canvasSize.y / 8)
	renderTexture:setFilterMode(solo.TextureFilter_Nearest, solo.TextureFilter_Nearest)
	renderTexture:setWrapMode(solo.TextureWrapMode_Clamp, solo.TextureWrapMode_Clamp)

	local renderTarget = resourceManager:getOrCreateRenderTarget("test")
	renderTarget:setTextures({ renderTexture })

	local renderTargetMaterial = resourceManager:getOrCreateMaterial(materials.texEffect)
	renderTargetMaterial:setPolygonFace(solo.PolygonFace_All)
	renderTargetMaterial:getParameter("worldViewProjMatrix"):bindValue(solo.AutoBinding_WorldViewProjectionMatrix)
	renderTargetMaterial:getParameter("mainTex"):setTexture(renderTexture)

	return
	{
		renderTarget = renderTarget,
		material = renderTargetMaterial
	}
end


function initObjects(models, materials, rtInfo)
	local canvasSize = device:getCanvasSize()

	-- Monkey
	local node = scene:createNode()
	local renderer = node:addStandardComponent("ModelRenderer")
	renderer:setModel(models.monkey)
	renderer:setMaterial(materials.texWithLighting)
	node:findStandardComponent("Transform"):setLocalPosition(solo.Vector3.zero())
	node:addComponent(createLocalXRotator())

	-- RTT quad
	local parent = scene:createNode()
	parent:findStandardComponent("Transform"):setLocalPosition(solo.Vector3(-2, 2, -2))
	parent:addComponent(createWorldYRotator())
	initAxesModel(parent, models, materials)

	local quad = createQuad()
	quad:findStandardComponent("ModelRenderer"):setMaterialForMesh(0, rtInfo.material)
	local quadTransform = quad:findStandardComponent("Transform")
	quadTransform:setParent(parent:findStandardComponent("Transform"))
	quadTransform:setLocalPosition(solo.Vector3(5, 2, -5))
	quadTransform:setLocalScale(solo.Vector3(5, 5 * canvasSize.y / canvasSize.x, 1))
	quad:addComponent(createTargeter(node:findStandardComponent("Transform"))) -- monkey

	-- Textured quad
	parent = scene:createNode()
	parent:findStandardComponent("Transform"):setLocalPosition(solo.Vector3(5, 0, 0))
	parent:addComponent(createWorldYRotator())
	initAxesModel(parent, models, materials)

	quad = createQuad()
	quad:addComponent(createLocalXRotator());
	quad:findStandardComponent("Transform"):setParent(parent:findStandardComponent("Transform"))
	quad:findStandardComponent("Transform"):setLocalPosition(solo.Vector3(2, 0, 0))
	quad:findStandardComponent("ModelRenderer"):setMaterialForMesh(0, materials.tex)

	-- Box
	node = createQuad()
	rebuildToBoxMesh(node)
	node:findStandardComponent("ModelRenderer"):setMaterialForMesh(0, materials.checker)
	node:findStandardComponent("Transform"):setLocalPosition(solo.Vector3(-5, 0, 0))
	node:addComponent(createWorldYRotator())
end


function rebuildToBoxMesh(node)
	local model = node:findStandardComponent("ModelRenderer"):getModel()
	local mesh = model:getMesh(0)

	mesh:setVertices(
	{
		solo.Vector3(-1, -1, 1),
		solo.Vector3(-1, 1, 1),
		solo.Vector3(1, 1, 1),
		solo.Vector3(1, -1, 1),

		solo.Vector3(-1, -1, -1),
		solo.Vector3(-1, 1, -1),
		solo.Vector3(-1, 1, 1),
		solo.Vector3(-1, -1, 1),

		solo.Vector3(1, -1, -1),
		solo.Vector3(1, 1, -1),
		solo.Vector3(-1, 1, -1),
		solo.Vector3(-1, -1, -1),

		solo.Vector3(1, -1, 1),
		solo.Vector3(1, 1, 1),
		solo.Vector3(1, 1, -1),
		solo.Vector3(1, -1, -1),

		solo.Vector3(-1, 1, 1),
		solo.Vector3(-1, 1, -1),
		solo.Vector3(1, 1, -1),
		solo.Vector3(1, 1, 1),

		solo.Vector3(-1, -1, -1),
		solo.Vector3(-1, -1, 1),
		solo.Vector3(1, -1, 1),
		solo.Vector3(1, -1, -1)
	})

	mesh:setUVs({
		solo.Vector2(0, 0),
		solo.Vector2(0, 1),
		solo.Vector2(1, 1),
		solo.Vector2(1, 0),
		solo.Vector2(0, 0),
		solo.Vector2(0, 1),
		solo.Vector2(1, 1),
		solo.Vector2(1, 0),
		solo.Vector2(0, 0),
		solo.Vector2(0, 1),
		solo.Vector2(1, 1),
		solo.Vector2(1, 0),
		solo.Vector2(0, 0),
		solo.Vector2(0, 1),
		solo.Vector2(1, 1),
		solo.Vector2(1, 0),
		solo.Vector2(0, 0),
		solo.Vector2(0, 1),
		solo.Vector2(1, 1),
		solo.Vector2(1, 0),
		solo.Vector2(0, 0),
		solo.Vector2(0, 1),
		solo.Vector2(1, 1),
		solo.Vector2(1, 0)
	})

	mesh:setIndices(
	{
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 6, 7,
		8, 9, 10,
		8, 10, 11,
		12, 13, 14,
		12, 14, 15,
		16, 17, 18,
		16, 18, 19,
		20, 21, 22,
		20, 22, 23
	})
end


function initAxesModel(node, models, materials)
	local renderer = node:addStandardComponent("ModelRenderer")
	renderer:setModel(models.axes)
	renderer:setMaterialForMesh(0, materials.blue)
	renderer:setMaterialForMesh(1, materials.green)
	renderer:setMaterialForMesh(2, materials.white)
	renderer:setMaterialForMesh(3, materials.red)
end


function createQuad()
	local mesh = resourceManager:getOrCreateMesh()
	mesh:setVertices({
		solo.Vector3(-1, -1, 0),
		solo.Vector3(-1, 1, 0),
		solo.Vector3(1, 1, 0),
		solo.Vector3(1, -1, 0)
	})
	mesh:setNormals({
		solo.Vector3(0, 0, -1),
		solo.Vector3(0, 0, -1),
		solo.Vector3(0, 0, -1),
		solo.Vector3(0, 0, -1)
	})
	mesh:setUVs({
		solo.Vector2(0, 0),
		solo.Vector2(0, 1),
		solo.Vector2(1, 1),
		solo.Vector2(1, 0)
	})
	mesh:setIndices({
		0, 1, 2,
		0, 2, 3
	})

	local model = resourceManager:getOrCreateModel()
	model:addMesh(mesh)

	local node = scene:createNode()
	node:addStandardComponent("ModelRenderer"):setModel(model)

	return node
end


function initCameras(rtInfo)
	local canvasSize = device:getCanvasSize()
	local offscreenCameraNode = scene:createNode()
	local offscreenCameraTransform = offscreenCameraNode:findStandardComponent("Transform")
	offscreenCameraTransform:setLocalPosition(solo.Vector3(0, 0, 10))
	local offscreenCamera = offscreenCameraNode:addStandardComponent("Camera")
	offscreenCamera:setClearColor(1, 1, 1, 1)
	offscreenCamera:setNear(0.05)
	offscreenCamera:setRenderTarget(rtInfo.renderTarget)
	offscreenCamera:setViewport(0, 0, canvasSize.x / 8, canvasSize.y / 8)

	local mainCameraNode = scene:createNode()
	local mainCameraTransform = mainCameraNode:findStandardComponent("Transform")
	mainCameraTransform:setLocalPosition(solo.Vector3(0, 2, 15))
	mainCameraNode:addComponent(createSpectator())
	mainCameraNode:addComponent(createEscapeWatcher())
	local mainCamera = mainCameraNode:addStandardComponent("Camera")
	mainCamera:setClearColor(0, 0.6, 0.6, 1)
	mainCamera:setNear(0.05)
end


function init()
	local materials = initMaterials()
	local models = initModels()
	local rtInfo = initRenderTarget(materials)
	initObjects(models, materials, rtInfo)
	initCameras(rtInfo)
end


engine = solo.Engine.create(solo.EngineCreationArgs(solo.EngineMode_OpenGL, 800, 600))
engine:setCallback(createEngineCallback())
engine:run()
