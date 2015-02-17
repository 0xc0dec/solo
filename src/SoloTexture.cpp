#include "SoloTexture.h"
#include "platform/SoloOpenGLTexture2D.h"

using namespace solo;


shared<Texture2D> TextureFactory::create2D(Texture::Format format, std::vector<char> data, unsigned width, unsigned height, bool generateMipmaps)
{
	return NEW2(OpenGLTexture2D, format, data, width, height, generateMipmaps);
}