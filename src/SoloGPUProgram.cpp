#include "SoloGPUProgram.h"

using namespace solo;


GPUProgram::GPUProgram(const str &vsShaderSrc, const str &fsShaderSrc)
	: _valid(false)
{
	auto allSources = vsShaderSrc;
	allSources.append(fsShaderSrc);
	_id = computeHash(allSources);
}


GPUProgram::~GPUProgram()
{
}


void GPUProgram::_appendToLog(const str &newLog)
{
	_log.append(newLog).append("\n");
}