local vsBasic = [[
	#version 330 core

	layout (location = 0) in vec4 position;
	layout (location = 1) in vec3 normal;
	layout (location = 2) in vec2 uv;

	uniform mat4 worldViewProjMatrix;
	out vec2 uv0;
	out vec3 n;

	void main()
	{
		gl_Position = worldViewProjMatrix * position;
		uv0 = uv;
	}
]]

local fsColor = [[
	#version 330 core

	uniform vec4 color;

	in vec2 uv0;
	out vec4 fragColor;

	void main()
	{
		fragColor = color;
	}
]]

local fsTexture =[[
	#version 330 core

	uniform sampler2D mainTex;

	in vec2 uv0;
	out vec4 fragColor;

	void main()
	{
		fragColor = texture(mainTex, uv0);
	}
]]

local fsChecker = [[
	#version 330 core

	in vec2 uv0;
	uniform vec4 color;
	out vec4 fragColor;

	void main()
	{
		float xfloor = floor(uv0.x / 0.2);
		float yfloor = floor(uv0.y / 0.2) + 1;
		if (mod(xfloor, 2) == 0 && mod(yfloor, 2) == 0)
			fragColor = vec4(0, 0, 0, 1);
		else if (mod(xfloor, 2) > 0 && mod(yfloor, 2) > 0)
			fragColor = vec4(0, 0, 0, 1);
		else
			fragColor = color;
	}
]]

local vsBasicLighting = [[
	#version 330 core

	layout (location = 0) in vec4 position;
	layout (location = 1) in vec3 normal;
	layout (location = 2) in vec2 uv;

	uniform mat4 worldViewProjMatrix;
	uniform mat4 normalMatrix;
	out vec2 uv0;
	out vec3 n;

	void main()
	{
		gl_Position = worldViewProjMatrix * position;
		uv0 = uv;
		n = normalize((normalMatrix * vec4(normal, 1)).xyz);
	}
]]

local fsTextureWithLighting = [[
	#version 330 core

	uniform sampler2D mainTex;

	in vec2 uv0;
	in vec3 n;
	out vec4 fragColor;

	void main()
	{
		vec4 color = texture(mainTex, uv0);
		fragColor = color * dot(vec3(1, 1, 1), n) / (length(vec3(1, 1, 1)) * length(n));
	}
]]


callback =
{
	onDeviceCloseRequested = function()
		return true
	end,

	onEngineStarted = function()
		local _, err = pcall(init)
		if err then
			print(err)
		end
	end
}


function createEscapeWatcher()
	return
	{
		typeId = "EscapeWatcher",

		update = function()
			if device:isKeyPressed(solo.KeyCode_Escape, true) then
				device:requestShutdown()
			end
		end
	}
end


function createSpectator()
	return
	{
		typeId = "Spectator",

		init = function(self)
			self.transform = self.node:findStandardComponent("Transform")
		end,

		update = function(self)
			local mouseMotion = device:getMouseMotion();
			local dt = device:getTimeDelta();

			if device:isMouseButtonDown(solo.MouseButton_Right, true) then
				device:setCursorCaptured(true)
			end
			if device:isMouseButtonReleased(solo.MouseButton_Right) then
				device:setCursorCaptured(false)
			end

			if device:isMouseButtonDown(solo.MouseButton_Right, false) then
				if mouseMotion.x ~= 0 then
					self.transform:rotateAxisAngle(solo.Vector3.unitY(), 0.5 * dt * -mouseMotion.x, solo.TransformSpace_World)
				end
				if mouseMotion.y ~= 0 then
					self.transform:rotateAxisAngle(solo.Vector3.unitX(), 0.5 * dt * -mouseMotion.y, solo.TransformSpace_Self)
				end
			end

			local movement = solo.Vector3.zero()
			if device:isKeyPressed(solo.KeyCode_W, false) then movement:addVector3(self.transform:getLocalForward()) end
			if device:isKeyPressed(solo.KeyCode_S, false) then movement:addVector3(self.transform:getLocalBack()) end
			if device:isKeyPressed(solo.KeyCode_A, false) then movement:addVector3(self.transform:getLocalLeft()) end
			if device:isKeyPressed(solo.KeyCode_D, false) then movement:addVector3(self.transform:getLocalRight()) end
			if device:isKeyPressed(solo.KeyCode_Q, false) then movement:addVector3(self.transform:getLocalDown()) end
			if device:isKeyPressed(solo.KeyCode_E, false) then movement:addVector3(self.transform:getLocalUp()) end
			movement:normalize()
			movement:multScalar(dt * 10)
			self.transform:translateLocal(movement)
		end
	}
end


function createLocalXRotator()
	return
	{
		typeId = "RotatorAroundLocalXAxis",

		init = function(self)
			self.transform = self.node:findStandardComponent("Transform")
		end,

		update = function(self)
			local angle = device:getTimeDelta() * 1.3
			self.transform:rotate(solo.Vector3.unitX(), angle, solo.TransformSpace_Self)
		end
	}
end


function createWorldYRotator()
	return
	{
		typeId = "RotatorAroundWorldYAxis",

		init = function(self)
			self.transform = self.node:findStandardComponent("Transform")
		end,

		update = function(self)
			local angle = device:getTimeDelta()
			self.transform:rotate(solo.Vector3.unitY(), angle, solo.TransformSpace_World)
		end
	}
end


function createTargeter(target)
	return
	{
		typeId = "Targeter",

		init = function(self)
			self.transform = self.node:findStandardComponent("Transform")
		end,

		update = function(self)
			self.transform:lookAt(target:getWorldPosition(), solo.Vector3.unitY())
		end
	}
end


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

	local colorEffect = resourceManager:getOrCreateEffect(vsBasic, fsColor)
	local redMaterial = resourceManager:getOrCreateMaterial(colorEffect)
	-- redMaterial:setPolygonFace(solo.PolygonFace_All) TODO
	redMaterial:getParameter("worldViewProjMatrix"):bindValue(solo.AutoBinding_WorldViewProjectionMatrix)
	redMaterial:getParameter("color"):setVector4(solo.Vector4(1, 0, 0, 1))

	local greenMaterial = resourceManager:getOrCreateMaterial(colorEffect)
	-- greenMaterial:setPolygonFace(solo.PolygonFace_All) TODO
	greenMaterial:getParameter("worldViewProjMatrix"):bindValue(solo.AutoBinding_WorldViewProjectionMatrix)
	greenMaterial:getParameter("color"):setVector4(solo.Vector4(0, 1, 0, 1))

	local blueMaterial = resourceManager:getOrCreateMaterial(colorEffect)
	-- blueMaterial:setPolygonFace(solo.PolygonFace_All) TODO
	blueMaterial:getParameter("worldViewProjMatrix"):bindValue(solo.AutoBinding_WorldViewProjectionMatrix)
	blueMaterial:getParameter("color"):setVector4(solo.Vector4(0, 0, 1, 1))

	local whiteMaterial = resourceManager:getOrCreateMaterial(colorEffect)
	-- white:setPolygonFace(solo.PolygonFace_All) TODO
	whiteMaterial:getParameter("worldViewProjMatrix"):bindValue(solo.AutoBinding_WorldViewProjectionMatrix)
	whiteMaterial:getParameter("color"):setVector4(solo.Vector4(1, 1, 1, 1))

	local texEffect = resourceManager:getOrCreateEffect(vsBasic, fsTexture)
	local texMaterial = resourceManager:getOrCreateMaterial(texEffect)
	-- texMaterial:setPolygonFace(PolygonFace::All); TODO
	texMaterial:getParameter("worldViewProjMatrix"):bindValue(solo.AutoBinding_WorldViewProjectionMatrix)
	texMaterial:getParameter("mainTex"):setTexture(tex1)

	local checkerEffect = resourceManager:getOrCreateEffect(vsBasic, fsChecker)
	local checkerMaterial = resourceManager:getOrCreateMaterial(checkerEffect)
	-- checkerMaterial:setPolygonFace(PolygonFace::All); TODO
	checkerMaterial:getParameter("worldViewProjMatrix"):bindValue(solo.AutoBinding_WorldViewProjectionMatrix);
	checkerMaterial:getParameter("color"):setVector4(solo.Vector4(1, 1, 0, 1))

	local texWithLightingEffect = resourceManager:getOrCreateEffect(vsBasicLighting, fsTextureWithLighting)
	local texWithLightingMaterial = resourceManager:getOrCreateMaterial(texWithLightingEffect)
	-- texWithLightingMaterial:setPolygonFace(PolygonFace_All) TODO
	texWithLightingMaterial:getParameter("worldViewProjMatrix"):bindValue(solo.AutoBinding_WorldViewProjectionMatrix)
	texWithLightingMaterial:getParameter("normalMatrix"):bindValue(solo.AutoBinding_InverseTransposedWorldMatrix)
	texWithLightingMaterial:getParameter("mainTex"):setTexture(tex2)

	return
	{
		red = redMaterial,
		green = greenMaterial,
		blue = blueMaterial,
		white = whiteMaterial,
		tex = texMaterial,
		checker = checkerMaterial,
		texWithLighting = texWithLightingMaterial
	}
end


function init()
	local auxNode = scene:createNode()
	auxNode:addComponent(createEscapeWatcher())
	auxNode:addComponent(createSpectator())

	initMaterials()
end

engine = solo.Engine.create(solo.EngineCreationArgs(solo.EngineMode_OpenGL, 800, 600))
device = engine:getDevice()
scene = engine:getScene()
resourceManager = engine:getResourceManager()
engine:setCallback(callback)
engine:run()
