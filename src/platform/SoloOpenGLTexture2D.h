#pragma once

#include "SoloTexture2D.h"

namespace solo
{
	class OpenGLTexture2D: public Texture2D
	{
	private:
		friend class TextureFactory;

		OpenGLTexture2D();
		OpenGLTexture2D(const OpenGLTexture2D& other) = delete;
		OpenGLTexture2D(OpenGLTexture2D&& other) = delete;
		OpenGLTexture2D& operator=(const OpenGLTexture2D& other) = delete;
		OpenGLTexture2D& operator=(OpenGLTexture2D&& other) = delete;
	};
}