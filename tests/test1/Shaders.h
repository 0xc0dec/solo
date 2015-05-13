#pragma once

const char *vsBasic = R"s(
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
)s";


const char *vsBasicLighting = R"s(
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
)s";

const char *fsTextureWithLighting = R"s(
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
)s";

const char *fsChecker = R"s(
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
)s";

const char *fsTexture = R"s(
	#version 330 core
	
	uniform sampler2D mainTex;
	
	in vec2 uv0;
	out vec4 fragColor;
	
	void main()
	{
		fragColor = texture(mainTex, uv0);
	}
)s";

// taken from ShaderToy
const char* fsRare = R"s(
	#version 330 core
	#define sphsize .15

	const float dist = 2.;
	const float perturb = .2;
	const float windspeed = .25;
	const float steps = 150.;
	const float stepsize = .03;
	const float brightness = .2;
	const float fade = .003;
	const float glow = 2.5;
	const int iterations = 10;
	const float fractparam = .85;
	const vec3 offset = vec3(1.5, 2., -0.);
	const vec3 planetcolor = vec3(1., 1., .5);
	
	in vec2 uv0;
	out vec4 fragColor;
	uniform float time;
	uniform float canvasWidth;
	uniform float canvasHeight;
	
	float rand(vec2 p) {return fract(sin(dot(p,vec2(2132.342,4323.343)))*1325.2158);}
	
	float surf(vec2 p)
	{
		p = abs(fract((p + vec2(.5, .3))*vec2(.2, .4)) - .5);
		float av = 0.;
		for (int i = 0; i<15; i++) {
			p = abs(p) / dot(p, p) - .65;
			av += min(20., length(p*p));
		}
		return av*.1;
	}
	
	float wind(vec3 p, float a)
	{
		mat2 rota = mat2(cos(a), sin(a), -sin(a), cos(a));
		float d = max(0., dist - max(0., length(p*p*p) - sphsize) / sphsize) / dist;
		p -= d*normalize(p * p * p)*perturb;
		p += vec3(0., -time * windspeed, 0.);
		p.xy *= rota;
		p = abs(fract((p + offset)*.1) - .5);
		for (int i = 0; i<iterations; i++)
			p = abs(p) / dot(p, p) - fractparam;
		return pow(length(p), 1.5)*(1. + d*glow) + d*glow;
	}
	
	void main()
	{
		vec3 dir = vec3(uv0, 1.);
		dir.x *= canvasWidth / canvasHeight;
		vec3 from = vec3(sin(time * .5), 0., -2.);
		float v = 0., l = -0.0001, t = time * windspeed*.2;
		for (float r = 10.; r<steps; r++)
		{
			vec3 p = from + r*dir*stepsize;
			float k = min(200., wind(p, r*0.1));
			v += k*max(0., 1. - r*fade);
			l = pow(max(.2, dot(normalize(p*p), normalize(vec3(-1., 1.5, -0.5)))), 2.);
		}
		v /= steps; v *= brightness;
		vec3 col = vec3(v*v, v*v*v, v*1.5) + l*planetcolor;
		fragColor = vec4(col, 1.0);
	}
)s";
