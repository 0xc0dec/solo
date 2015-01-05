#include "SoloVideoDriver.h"
#include "platform/SoloOpenGLVideoDriver.h"

using namespace solo;


VideoDriver::VideoDriver()
{
}


VideoDriver::~VideoDriver()
{
}


ptr<VideoDriver> VideoDriver::create()
{
	return OpenGLVideoDriver::create();
}


ptr<IEffect> VideoDriver::createEffect(const std::string &vsSrc, const std::string &fsSrc)
{
	auto result = Effect::create(vsSrc, fsSrc);
	_effects.push_back(result);
	return result;
}
