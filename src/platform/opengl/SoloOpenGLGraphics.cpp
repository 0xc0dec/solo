#include "SoloOpenglGraphics.h"
#include "SoloMaterial.h"
#include "SoloDevice.h"
#include "SoloResourceManager.h"
#include "SoloRenderTarget.h"
#include "SoloMesh.h"
#include "SoloRenderContext.h"
#include <GL/glew.h>

using namespace solo;


OpenGLGraphics::OpenGLGraphics(Device* device):
	Graphics(device)
{
}


void OpenGLGraphics::renderMaterialSurfaceToTarget(Material* material, RenderTarget* target)
{
	drawMaterial(material, target);
}


void OpenGLGraphics::renderMaterialSurfaceToScreen(Material* material)
{
	drawMaterial(material, nullptr);
}


void OpenGLGraphics::drawMaterial(Material* material, RenderTarget* target)
{
	if (!quadMesh) // TODO move this to some kind of "init" function
	{
		quadMesh = device->getResourceManager()->getOrCreateMesh("solo/internal/quad");
		quadMesh->rebuildAsQuad();
	}

	auto depthTestEnabled = material->isDepthTestEnabled();
	material->setDepthTestEnabled(false);

	if (target)
		target->bind();

	auto viewportSize = target ? target->getColorAttachmentSize() : device->getCanvasSize();
	glViewport(
		static_cast<GLuint>(0),
		static_cast<GLuint>(0),
		static_cast<GLuint>(viewportSize.x),
		static_cast<GLuint>(viewportSize.y));

	RenderContext ctx;
	material->bind(ctx);
	quadMesh->draw();
	material->unbind(ctx);

	material->setDepthTestEnabled(depthTestEnabled);

	if (target)
		target->unbind();
}
