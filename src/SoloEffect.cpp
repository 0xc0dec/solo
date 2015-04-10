#include "SoloEffect.h"
#include "platform/SoloGLSLEffect.h"

using namespace solo;


shared<Effect> EffectFactory::create(const std::string& vsSrc, const std::string& fsSrc)
{
	return NEW2(GLSLEffect, vsSrc, fsSrc);
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

