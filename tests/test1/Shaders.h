#pragma once

const char *vsBasic =
" \
#version 330 core\n \
\
layout (location = 0) in vec4 position;\n \
layout (location = 2) in vec2 uv;\n \
\
uniform mat4 worldViewProj;\n \
\
out vec2 uv0;\n \
\
void main()\n \
{\n \
	gl_Position = worldViewProj * position;\n \
	uv0 = uv;\n \
}";

const char *fsChecker =
" \
#version 330 core\n \
\
in vec2 uv0;\n \
\
uniform vec4 color;\n \
out vec4 fragColor;\n \
\
void main()\n \
{\n \
	float xfloor = floor(uv0.x / 0.2);\n \
	float yfloor = floor(uv0.y / 0.2) + 1;\n \
	if (mod(xfloor, 2) == 0 && mod(yfloor, 2) == 0)\n \
		fragColor = vec4(0, 0, 0, 1);\n \
	else if (mod(xfloor, 2) > 0 && mod(yfloor, 2) > 0)\n \
		fragColor = vec4(0, 0, 0, 1);\n \
	else\n \
		fragColor = color;\n \
}";

const char *fsTexture =
"\
#version 330 core\n \
\
uniform sampler2D mainTex;\n \
\
in vec2 uv0;\n \
out vec4 fragColor;\n \
\
void main()\n \
{\n \
	fragColor = texture(mainTex, uv0);\n \
}";

// taken from ShaderToy
const char* fsRare =
" \
#version 330 core\n \
#define sphsize .15\n \
const float dist = 2.;\n \
const float perturb = .2;\n \
const float windspeed = .25;\n \
const float steps = 150.;\n \
const float stepsize = .03;\n \
const float brightness = .2;\n \
const float fade = .003;\n \
const float glow = 2.5;\n \
const int iterations = 10;\n \
const float fractparam = .85;\n \
const vec3 offset = vec3(1.5, 2., -0.);\n \
const vec3 planetcolor = vec3(1., 1., .5);\n \
\
in vec2 uv0;\n \
out vec4 fragColor;\n \
uniform float time;\n \
uniform float canvasWidth;\n \
uniform float canvasHeight;\n \
\
float rand(vec2 p) {return fract(sin(dot(p,vec2(2132.342,4323.343)))*1325.2158);}\n \
\
float surf(vec2 p)\n \
{\n \
	p = abs(fract((p + vec2(.5, .3))*vec2(.2, .4)) - .5);\n \
	float av = 0.;\n \
	for (int i = 0; i<15; i++) {\n \
		p = abs(p) / dot(p, p) - .65;\n \
		av += min(20., length(p*p));\n \
	}\n \
	return av*.1;\n \
}\n \
\
float wind(vec3 p, float a)\n \
{\n \
	mat2 rota = mat2(cos(a), sin(a), -sin(a), cos(a));\n \
	float d = max(0., dist - max(0., length(p*p*p) - sphsize) / sphsize) / dist;\n \
	p -= d*normalize(p * p * p)*perturb;\n \
	p += vec3(0., -time * windspeed, 0.);\n \
	p.xy *= rota;\n \
	p = abs(fract((p + offset)*.1) - .5);\n \
	for (int i = 0; i<iterations; i++)\n \
		p = abs(p) / dot(p, p) - fractparam;\n \
	return pow(length(p), 1.5)*(1. + d*glow) + d*glow;\n \
}\n \
\
void main()\n \
{\n \
	//vec2 uv = gl_FragCoord.xy / vec2(canvasWidth, canvasHeight)-.5;\n \
	vec3 dir = vec3(uv0, 1.);\n \
	dir.x *= canvasWidth / canvasHeight;\n \
	vec3 from = vec3(sin(time * .5), 0., -2.);\n \
	float v = 0., l = -0.0001, t = time * windspeed*.2;\n \
	for (float r = 10.; r<steps; r++)\n \
	{\n \
		vec3 p = from + r*dir*stepsize;\n \
		float k = min(200., wind(p, r*0.1));\n \
		v += k*max(0., 1. - r*fade);\n \
		l = pow(max(.2, dot(normalize(p*p), normalize(vec3(-1., 1.5, -0.5)))), 2.);\n \
	}\n \
	v /= steps; v *= brightness;\n \
	vec3 col = vec3(v*v, v*v*v, v*1.5) + l*planetcolor;\n \
	fragColor = vec4(col, 1.0);\n \
}";