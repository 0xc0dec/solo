#include "SoloOpenGLMaterial.h"
#include <GL/glew.h>

using namespace solo;


OpenGLMaterial::OpenGLMaterial(shared<Effect> effect):
	Material(effect)
{
}


void OpenGLMaterial::applyState()
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

	GLenum depthFunc;
	switch (depthPassFunc)
	{
		case DepthPassFunction::Never:
			depthFunc = GL_NEVER;
			break;
		case DepthPassFunction::Less:
			depthFunc = GL_LESS;
			break;
		case DepthPassFunction::Equal:
			depthFunc = GL_EQUAL;
			break;
		case DepthPassFunction::LEqual:
			depthFunc = GL_LEQUAL;
			break;
		case DepthPassFunction::Greater:
			depthFunc = GL_GREATER;
			break;
		case DepthPassFunction::NotEqual:
			depthFunc = GL_NOTEQUAL;
			break;
		case DepthPassFunction::GEqual:
			depthFunc = GL_GEQUAL;
			break;
		case DepthPassFunction::Always:
			depthFunc = GL_ALWAYS;
			break;
		default:
			return;
	}

	glDepthFunc(depthFunc);
	glDepthMask(depthWrite ? GL_TRUE : GL_FALSE);

	if (depthTest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}
