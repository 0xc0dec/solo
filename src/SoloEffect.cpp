#include "SoloStubEffect.h"
#include "SoloEngineCreationArgs.h"
#include "platform/SoloGLSLEffect.h"

using namespace solo;


shared<Effect> EffectFactory::create(EngineMode engineMode, const std::string& vsSrc, const std::string& fsSrc)
{
	if (engineMode == EngineMode::OpenGL)
		return NEW2(GLSLEffect, vsSrc, fsSrc);
	return NEW2(StubEffect);
}


EffectVariable* Effect::findVariable(const std::string& name) const
{
	auto where = variables.find(name);
	if (where != variables.end())
		return where->second.get();
	return nullptr;
}
