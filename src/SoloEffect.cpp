#include "SoloEffect.h"
#include "platform/SoloGLSLEffect.h"

using namespace solo;


Effect::Effect(const std::string &vsSrc, const std::string &fsSrc):
	_valid(false),
	_id(0)
{
	_id = getHash(vsSrc + fsSrc);
}


ptr<Effect> Effect::create(const std::string& vsSrc, const std::string& fsSrc)
{
	// GLSL is the only option at the moment
	return NEW<GLSLEffect>(vsSrc, fsSrc);
}


void Effect::appendToLog(const std::string &newLog)
{
	_log.append(newLog).append("\n");
}


bool Effect::isValid() const
{
	return _valid;
}


std::string Effect::getLog() const
{
	return _log;
}


size_t Effect::getId() const
{
	return _id;
}
