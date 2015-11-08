#include "SoloGraphics.h"
#include "SoloDevice.h"
#include "SoloResourceManager.h"
#include "SoloMaterial.h"
#include "SoloMaterialParameter.h"
#include "SoloRenderTarget.h"
#include "SoloTexture2D.h"
#include "SoloMesh.h"
#include "SoloRenderContext.h"
#include <GL/glew.h>

using namespace solo;


shared<Graphics> GraphicsFactory::create(Device* device)
{
	return SL_NEW2(Graphics, device);
}


Graphics::Graphics(Device *device):
	device(device)
{
}


void Graphics::renderImageToTarget(shared<Texture2D> source, RenderTarget *target, Material *material, const std::string &textureParameterName)
{
	if (!quadMesh) // TODO move this to some kind of "init" function
		quadMesh = device->getResourceManager()->getOrCreatePrimitiveMesh(PrimitiveMeshType::Quad, "solo/internal/quad");

	material->getParameter(textureParameterName.empty() ? "mainTexture" : textureParameterName)->setTexture(source);
	if (target)
		target->bind();
	glDisable(GL_DEPTH_TEST);
	auto size = device->getCanvasSize();
	glViewport(
		static_cast<GLuint>(0),
		static_cast<GLuint>(0),
		static_cast<GLuint>(size.x),
		static_cast<GLuint>(size.y));
	RenderContext ctx;
	material->bind(ctx);
	quadMesh->draw();
	material->unbind(ctx);
	glEnable(GL_DEPTH_TEST);
	if (target)
		target->unbind();
}


void Graphics::renderImageToScreen(shared<Texture2D> source, Material* material, const std::string& textureParameterName)
{
	if (!quadMesh) // TODO move this to some kind of "init" function
		quadMesh = device->getResourceManager()->getOrCreatePrimitiveMesh(PrimitiveMeshType::Quad, "solo/internal/quad");

	glDisable(GL_DEPTH_TEST);
	auto size = device->getCanvasSize();
	glViewport(
		static_cast<GLuint>(0),
		static_cast<GLuint>(0),
		static_cast<GLuint>(size.x),
		static_cast<GLuint>(size.y));
	material->getParameter(textureParameterName.empty() ? "mainTexture" : textureParameterName)->setTexture(source);
	RenderContext ctx;
	material->bind(ctx);
	quadMesh->draw();
	material->unbind(ctx);
	glEnable(GL_DEPTH_TEST);
}
