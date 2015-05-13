#include <png.h>
#include "SoloPNGTextureLoader.h"
#include "SoloFileSystem.h"
#include "SoloTexture2D.h"

using namespace solo;


struct PNGReadContext
{
	std::vector<byte>* bytes;
	unsigned int offset;
};


static void readCallback(png_structp png, png_bytep data, png_size_t length)
{
	auto context = reinterpret_cast<PNGReadContext*>(png_get_io_ptr(png));
	if (!context)
		png_error(png, "Error reading PNG.");
	memcpy(data, context->bytes->data() + context->offset, length);
	context->offset += length;
}


bool PNGTextureLoader::isLoadable(const std::string& url)
{
	return url.find(".png", url.size() - 5) != std::string::npos;
}


shared<Texture2D> PNGTextureLoader::load2D(const std::string& url)
{
	auto bytes = fs->readBytes(url);
	if (bytes.size() < 8 || png_sig_cmp(&bytes[0], 0, 8) != 0)
		THROW_FMT(EngineException, "Failed to read PNG file ", url);

	auto png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	auto info = png_create_info_struct(png);
	if (setjmp(png_jmpbuf(png)))
	{
		png_destroy_read_struct(&png, &info, nullptr);
		THROW_FMT(EngineException, "Failed to read PNG file ", url);
	}

	std::unique_ptr<PNGReadContext> context(new PNGReadContext{ &bytes, 8 });
	png_set_read_fn(png, reinterpret_cast<png_voidp>(context.get()), readCallback);
	png_set_sig_bytes(png, 8);
	png_read_png(png, info, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, nullptr);

	auto width = png_get_image_width(png, info);
	auto height = png_get_image_height(png, info);
	auto colorType = png_get_color_type(png, info);

	Texture::ColorFormat colorFormat;
	switch (colorType)
	{
	case PNG_COLOR_TYPE_RGB:
		colorFormat = Texture::ColorFormat::RGB;
		break;
	case PNG_COLOR_TYPE_RGBA:
		colorFormat = Texture::ColorFormat::RGBA;
		break;
	default:
		png_destroy_read_struct(&png, &info, nullptr);
		THROW_FMT(EngineException, "Unsupported PNG color type ", colorType);
	}

	auto stride = png_get_rowbytes(png, info);
	auto data = std::vector<byte>(stride * height);
	auto rows = png_get_rows(png, info);
	for (unsigned int i = 0; i < height; ++i)
		memcpy(data.data() + stride * (height - 1 - i), rows[i], stride);

	png_destroy_read_struct(&png, &info, nullptr);

	auto tex = TextureFactory::create2D();
	tex->setData(colorFormat, data, width, height);
	return tex;
}