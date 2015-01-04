#include "SoloGPUProgram.h"

using namespace solo;


GPUProgram::GPUProgram(const std::string &vsShaderSrc, const std::string &fsShaderSrc)
	: _valid(false)
{
	auto allSources = vsShaderSrc;
	allSources.append(fsShaderSrc);
	_id = getHash(allSources);
}


GPUProgram::~GPUProgram()
{
}


void GPUProgram::_appendToLog(const std::string &newLog)
{
	_log.append(newLog).append("\n");
}


bool GPUProgram::isValid() const
{
	return _valid;
}


std::string GPUProgram::getLog() const
{
	return _log;
}


size_t GPUProgram::getId() const
{
	return _id;
}