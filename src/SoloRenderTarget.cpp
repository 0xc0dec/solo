#include "SoloRenderTarget.h"
#include "platform/SoloOpenGLRenderTarget.h"

using namespace solo;


shared<RenderTarget> RenderTargetFactory::create()
{
	return NEW2(OpenGLRenderTarget);
}


void RenderTarget::setTexture(shared<Texture2D> texture)
{
	this->texture = texture;
	applyTexture();
}