#include "SoloDevice.h"
#include "platform/stub/SoloStubRenderTarget.h"
#include "platform/opengl/SoloOpenGLRenderTarget.h"

using namespace solo;


shared<RenderTarget> RenderTargetFactory::create(DeviceMode mode)
{
	if (mode == DeviceMode::OpenGL)
		return SL_NEW2(OpenGLRenderTarget);
	return SL_NEW2(StubRenderTarget);
}


void RenderTarget::setTextures(const std::vector<shared<Texture2D>>& textures)
{
	this->textures = textures;
	update();
}


std::vector<shared<Texture2D>> RenderTarget::getTextures() const
{
	return textures;
}