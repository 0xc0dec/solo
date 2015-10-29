#pragma once

#include "SoloCubeTexture.h"
#include <GL/glew.h>

namespace solo
{
	class OpenGLCubeTexture: public CubeTexture
	{
	public:
		virtual ~OpenGLCubeTexture();

		virtual void setData(CubeTextureFace face, ColorFormat format, const std::vector<uint8_t>& data, unsigned width, unsigned height) override;
		virtual void apply() override;
		virtual void generateMipmaps() override;

		GLuint getHandle() const;

	private:
		friend class TextureFactory;

		OpenGLCubeTexture();

		void bind();
		void unbind();

		GLuint handle = 0;
	};
}