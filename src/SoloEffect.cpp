#include "SoloEffect.h"
#include "SoloDevice.h"

using namespace solo;


Effect::Effect(const std::string& vsSrc, const std::string& fsSrc):
    renderer(Device::get()->getRenderer())
{
    handle = renderer->createProgram(vsSrc.c_str(), fsSrc.c_str());
}


sptr<Effect> Effect::create(const std::string& vsSrc, const std::string& fsSrc)
{
    return std::unique_ptr<Effect>(new Effect(vsSrc, fsSrc));
}


Effect::~Effect()
{
    if (!handle.empty())
        renderer->destroyProgram(handle);
}


void Effect::apply()
{
    renderer->setProgram(handle);
}
