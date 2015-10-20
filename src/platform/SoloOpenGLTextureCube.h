#pragma once

#include <GL/glew.h>
#include "SoloTextureCube.h"

namespace solo
{
	class OpenGLTextureCube: public TextureCube
	{
	public:
		virtual ~OpenGLTextureCube();

		virtual void setImageData(TextureCubeFace face, ColorFormat format, const std::vector<uint8_t>& data, unsigned width, unsigned height) override;
		virtual void apply() override;
		virtual void generateMipmaps() override;

		GLuint getHandle() const;

	private:
		friend class TextureFactory;

		OpenGLTextureCube();

		void bind();
		void unbind();

		GLenum toGLColorFormat(ColorFormat format);
		GLenum toGLWrapMode(TextureWrapMode mode);
		GLenum toGLFilter(TextureFilter filter);

		GLuint handle = 0;
	};
}