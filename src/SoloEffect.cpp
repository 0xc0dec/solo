#include "SoloEffect.h"
#include "platform/SoloGLSLEffect.h"

using namespace solo;


Effect::Effect(const std::string &vsSrc, const std::string &fsSrc):
	valid(false),
	id(0)
{
	id = getHash(vsSrc + fsSrc);
}


ptr<Effect> Effect::create(const std::string& vsSrc, const std::string& fsSrc)
{
	// GLSL is the only option at the moment
	return GLSLEffect::create(vsSrc, fsSrc);
}


void Effect::appendToLog(const std::string &newLog)
{
	log.append(newLog).append("\n");
}


bool Effect::isValid() const
{
	return valid;
}


std::string Effect::getLog() const
{
	return log;
}


size_t Effect::getId() const
{
	return id;
}
