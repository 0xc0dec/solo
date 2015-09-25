#pragma once

#include <GL/glew.h>
#include "SoloTexture2D.h"

namespace solo
{
	class OpenGLTexture2D: public Texture2D
	{
	public:
		virtual ~OpenGLTexture2D();

		virtual void apply() override;
		virtual void generateMipmaps() override;
		virtual void *getNativeHandle() override;

	protected:
		virtual void applyData(ColorFormat format, const std::vector<uint8_t>& data, unsigned width, unsigned height) override;

	private:
		friend class TextureFactory;

		OpenGLTexture2D();

		void bind();
		void unbind();

		GLenum toGLColorFormat(ColorFormat format);
		GLenum toGLWrapMode(TextureWrapMode mode);
		GLenum toGLFilter(TextureFilter filter);

		GLuint handle = 0;
	};
}