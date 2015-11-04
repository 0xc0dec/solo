#include "SoloGraphics.h"
#include "SoloDevice.h"

using namespace solo;


shared<Graphics> GraphicsFactory::create(Device* device)
{
	return SL_NEW2(Graphics, device);
}


Graphics::Graphics(Device *device):
	device(device),
	resourceManager(device->getResourceManager())
{
	// TODO grab unit quad mesh
}


void Graphics::renderImage(Texture2D* source, RenderTarget* target, Material* material)
{
	// TODO render quad
}
