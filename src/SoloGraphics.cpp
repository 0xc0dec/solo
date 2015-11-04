#include "SoloGraphics.h"
#include "SoloDevice.h"
#include "SoloResourceManager.h"
#include "SoloMaterial.h"
#include "SoloMaterialParameter.h"
#include "SoloRenderTarget.h"
#include "SoloTexture2D.h"
#include "SoloMesh.h"

using namespace solo;


shared<Graphics> GraphicsFactory::create(Device* device)
{
	return SL_NEW2(Graphics, device);
}


Graphics::Graphics(Device *device)
{
	quadMesh = device->getResourceManager()->getOrCreatePrimitiveMesh(PrimitiveMeshType::Quad, "solo/internal/quad");
}


void Graphics::renderImage(shared<Texture2D> source, RenderTarget *target, Material *material, const std::string &textureParameterName)
{
	material->getParameter(textureParameterName)->setTexture(source);
	if (target)
		target->bind();
	quadMesh->draw();
	if (target)
		target->unbind();
}
