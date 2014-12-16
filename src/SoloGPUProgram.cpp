#include "SoloGPUProgram.h"

using namespace solo;


GPUProgram::GPUProgram(const std::string &vsShaderSrc, const std::string &fsShaderSrc)
	: _valid(false)
{
	auto allSources = vsShaderSrc;
	allSources.append(fsShaderSrc);
	_id = computeHash(allSources);
}


GPUProgram::~GPUProgram()
{
}


void GPUProgram::_appendToLog(const std::string &newLog)
{
	_log.append(newLog).append("\n");
}