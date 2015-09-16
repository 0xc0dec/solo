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

