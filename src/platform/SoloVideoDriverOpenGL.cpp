#include <GL/glew.h>
#include "SoloVideoDriverOpenGL.h"
#include "SoloGPUProgramGLSL.h"

using namespace solo;


ptr<IGPUProgram> VideoDriverOpenGL::createGPUProgram(const std::string &vsSrc, const std::string &fsSrc)
{
	auto program = NEW<GPUProgramGLSL>(vsSrc, fsSrc);
	_gpuPrograms.push_back(program);
	return program;
}


void VideoDriverOpenGL::setViewport(int left, int top, int width, int height)
{
	glViewport(static_cast<GLuint>(left), static_cast<GLuint>(top), static_cast<GLuint>(width), static_cast<GLuint>(height));
}