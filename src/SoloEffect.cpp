#include "SoloDevice.h"
#include "platform/stub/SoloStubEffect.h"
#include "platform/opengl/SoloGLSLEffect.h"

using namespace solo;


shared<Effect> Effect::create(DeviceMode mode, const std::string& vsSrc, const std::string& fsSrc)
{
	if (mode == DeviceMode::OpenGL)
		return SL_NEW2(GLSLEffect, vsSrc, fsSrc);
	return SL_NEW2(StubEffect);
}


EffectVariable* Effect::findVariable(const std::string& name) const
{
	auto where = variables.find(name);
	if (where != variables.end())
		return where->second.get();
	return nullptr;
}
