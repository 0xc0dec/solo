#pragma once

#include "SoloImage.h"
#include "SoloTexture.h"
#include <GL/glew.h>

namespace solo
{
	class OpenGLHelper
	{
	public:
		static GLenum convertToGLColorFormat(ColorFormat format);
		static GLenum convertToGLWrapMode(TextureWrapMode mode);
		static GLenum convertToGLFilter(TextureFiltering filter);
	};
}
