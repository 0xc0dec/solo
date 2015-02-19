#include <GL/glew.h>
#include "SoloOpenGLTextureSampler2D.h"

using namespace solo;


OpenGLTextureSampler2D::OpenGLTextureSampler2D(shared<Texture> texture):
	TextureSampler(texture)
{

}


void OpenGLTextureSampler2D::apply(RenderContext& context)
{

}