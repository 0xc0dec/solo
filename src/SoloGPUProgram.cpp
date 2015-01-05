#include "SoloGPUProgram.h"

using namespace solo;


GPUProgram::GPUProgram(const std::string &vsSrc, const std::string &fsSrc)
	: _valid(false)
{
	auto allSources = vsSrc;
	allSources.append(fsSrc);
	_id = getHash(allSources);
}


GPUProgram::~GPUProgram()
{
}


void GPUProgram::appendToLog(const std::string &newLog)
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