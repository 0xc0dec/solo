GLOBAL vsBasic = "
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
";

GLOBAL fsTexture = "
	#version 330 core
		
	uniform sampler2D mainTex;

	in vec2 uv0;
	out vec4 fragColor;

	void main()
	{
		fragColor = texture(mainTex, uv0);
	}
";

GLOBAL fsChecker = "
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
";

GLOBAL vsBasicLighting = "
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
";


GLOBAL fsTextureWithLighting = "
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
";