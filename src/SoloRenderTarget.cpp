#include "SoloRenderTarget.h"
#include "platform/SoloOpenGLRenderTarget.h"

using namespace solo;


shared<RenderTarget> RenderTargetFactory::create()
{
	return NEW2(OpenGLRenderTarget);
}