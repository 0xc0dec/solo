#include "SoloVideoDriver.h"
#include "SoloEffect.h"
#include "platform/SoloOpenGLVideoDriver.h"

using namespace solo;


ptr<VideoDriver> VideoDriver::create()
{
	return OpenGLVideoDriver::create();
}


ptr<Effect> VideoDriver::createEffect(const std::string &vsSrc, const std::string &fsSrc)
{
	auto result = Effect::create(vsSrc, fsSrc);
	_effects.push_back(result);
	return result;
}
