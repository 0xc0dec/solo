#include "SoloOpenGLMaterial.h"
#include <GL/glew.h>

using namespace solo;


OpenGLMaterial::OpenGLMaterial(shared<Effect> effect):
	Material(effect)
{
}


void OpenGLMaterial::applyFaceCull()
{
	switch (polygonFace)
	{
		case PolygonFace::All:
			glDisable(GL_CULL_FACE);
			break;
		case PolygonFace::CW:
			glFrontFace(GL_CW);
			glEnable(GL_CULL_FACE);
			break;
		case PolygonFace::CCW:
			glFrontFace(GL_CCW);
			glEnable(GL_CULL_FACE);
			break;
	}
}


void OpenGLMaterial::applyZWrite()
{
	glDepthMask(zwrite ? GL_TRUE : GL_FALSE);
}