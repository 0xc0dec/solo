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


size_t RenderTarget::getTextureCount() const
{
	return textures.size();
}


shared<Texture2D> RenderTarget::getTexture(size_t index) const
{
	auto total = textures.size();
	if (index >= total)
		SL_THROW_FMT(EngineException, "Invalid texture index ", index, " (expected less than ", total, ")");
	return textures[index];
}
