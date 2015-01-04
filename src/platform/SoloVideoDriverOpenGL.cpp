#include "SoloVideoDriverOpenGL.h"
#include "SoloGPUProgramGLSL.h"

using namespace solo;


ptr<IGPUProgram> VideoDriverOpenGL::createGPUProgram(const std::string &vsSrc, const std::string &fsSrc)
{
	auto program = NEW<GPUProgramGLSL>(vsSrc, fsSrc);
	_gpuPrograms.push_back(program);
	return program;
}
