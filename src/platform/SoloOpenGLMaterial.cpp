#include <GL/glew.h>
#include "SoloOpenGLMaterial.h"

using namespace solo;


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