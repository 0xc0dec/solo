#include "SoloRenderTarget.h"
#include "platform/SoloOpenGLRenderTarget.h"

using namespace solo;


shared<RenderTarget> RenderTargetFactory::create()
{
	return NEW2(OpenGLRenderTarget);
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