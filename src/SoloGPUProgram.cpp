#include "SoloGPUProgram.h"

using namespace solo;


GPUProgram::GPUProgram(const String &vsShaderSrc, const String &fsShaderSrc)
	: _valid(false)
{
	auto allSources = vsShaderSrc;
	allSources.append(fsShaderSrc);
	_id = computeHash(allSources);
}


GPUProgram::~GPUProgram()
{
}


void GPUProgram::_appendToLog(const String &newLog)
{
	_log.append(newLog).append("\n");
}