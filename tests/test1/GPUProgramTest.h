#ifndef __GPU_PROGRAM_TEST_H__
#define __GPU_PROGRAM_TEST_H__

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


class GPUProgramTest : public TestBase
{
public:
	virtual void run(IEngine *engine) override
	{
		auto program = engine->getDevice()->createGPUProgram(vs, fs);
		assert(program->isValid());
		assert(program->getLog().empty());
	}
};


#endif