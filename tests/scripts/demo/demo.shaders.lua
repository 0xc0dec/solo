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

local vsDirectDraw = [[
	#version 330 core

	layout (location = 0) in vec4 position;
	layout (location = 2) in vec2 uv;

	out vec2 uv0;
	out vec3 n;

	void main()
	{
		gl_Position = position;
		uv0 = uv;
	}
]]

local fsPostProcess = [[
	#version 330 core

	uniform sampler2D mainTex;

	in vec2 uv0;
	out vec4 fragColor;

	void main()
	{
		fragColor = texture(mainTex, uv0) / 2.0;
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

local vsSkybox = [[
	#version 330 core

	layout (location = 0) in vec4 position;

	uniform mat4 projMatrix;
	uniform mat4 worldViewMatrix;
	smooth out vec3 eyeDir;

	void main()
	{
		mat4 invProjMatrix = inverse(projMatrix);
		mat3 invModelViewMatrix = inverse(mat3(worldViewMatrix));
		vec3 unprojected = (invProjMatrix * position).xyz;

		eyeDir = invModelViewMatrix * unprojected;
		gl_Position = position;
	}
]]

local fsSkybox = [[
	#version 330 core

	uniform samplerCube mainTex;

	smooth in vec3 eyeDir;
	out vec4 fragColor;

	void main()
	{
		fragColor = texture(mainTex, eyeDir);
	}
]]

return
{
	vsBasic = vsBasic,
	vsDirectDraw = vsDirectDraw,
	fsColor = fsColor,
	fsTexture = fsTexture,
	fsChecker = fsChecker,
	vsBasicLighting = vsBasicLighting,
	fsTextureWithLighting = fsTextureWithLighting,
	vsSkybox = vsSkybox,
	fsSkybox = fsSkybox,
	fsPostProcess = fsPostProcess
}
