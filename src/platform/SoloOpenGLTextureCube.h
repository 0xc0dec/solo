#pragma once

#include "SoloTextureCube.h"
#include <GL/glew.h>

namespace solo
{
	class OpenGLTextureCube: public TextureCube
	{
	public:
		virtual ~OpenGLTextureCube();
		virtual void setData(TextureCubeFace face, ColorFormat format, const std::vector<uint8_t>& data, unsigned width, unsigned height) override;
		virtual void apply() override;
		virtual void generateMipmaps() override;

		GLuint getHandle() const;

	private:
		friend class TextureFactory;

		OpenGLTextureCube();

		void bind();
		void unbind();

		GLuint handle = 0;
	};
}