#pragma once

#include "TestBase.h"

const char * vs =
	"#version 330 core\n"

	"layout(location = 0) in vec3 vertexPositionModelSpace;"
	"layout(location = 1) in vec2 vertexUV;"

	"void main()\n"
	"{\n"
	"	gl_Position = vec4(vertexPositionModelSpace, 1);\n"
	"}";

const char *fs =
	"#version 330 core\n"

	"out vec3 color;\n"

	"void main()\n"
	"{\n"
	"	color = vec3(1);\n"
	"}";


class EffectsTest : public TestBase
{
public:
	virtual void run(IEngine *engine) override
	{
		auto effect = engine->getVideoDriver()->createEffect(vs, fs);
		assert(effect->isValid());
		assert(effect->getLog().empty());
	}
};
