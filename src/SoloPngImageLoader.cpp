#include "SoloPngImageLoader.h"
#include "SoloFileSystem.h"
#include "SoloImage.h"
#include "SoloResourceManager.h"
#include <png.h>

using namespace solo;


struct PngReadContext
{
	std::vector<uint8_t>* bytes;
	unsigned int offset;
};


static void readCallback(png_structp png, png_bytep data, png_size_t length)
{
	auto context = reinterpret_cast<PngReadContext*>(png_get_io_ptr(png));
	if (!context)
		png_error(png, "Error reading PNG");
	memcpy(data, context->bytes->data() + context->offset, length);
	context->offset += length;
}


PngImageLoader::PngImageLoader(FileSystem* fs, ResourceManager *resourceManager) :
	ImageLoader(fs, resourceManager)
{
}


bool PngImageLoader::isLoadable(const std::string& uri)
{
	return uri.find(".png", uri.size() - 5) != std::string::npos;
}


shared<Image> PngImageLoader::load(const std::string& uri)
{
	auto bytes = fs->readBytes(uri);
	if (bytes.size() < 8 || png_sig_cmp(&bytes[0], 0, 8) != 0)
		SL_THROW_FMT(EngineException, "Failed to read PNG file ", uri);

	auto png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	auto info = png_create_info_struct(png);
	if (setjmp(png_jmpbuf(png)))
	{
		png_destroy_read_struct(&png, &info, nullptr);
		SL_THROW_FMT(EngineException, "Failed to read PNG file ", uri);
	}

	std::unique_ptr<PngReadContext> context(new PngReadContext{ &bytes, 8 });
	png_set_read_fn(png, reinterpret_cast<png_voidp>(context.get()), readCallback);
	png_set_sig_bytes(png, 8);
	png_read_png(png, info, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, nullptr);

	auto width = png_get_image_width(png, info);
	auto height = png_get_image_height(png, info);
	auto colorType = png_get_color_type(png, info);

	ColorFormat colorFormat;
	switch (colorType)
	{
	case PNG_COLOR_TYPE_RGB:
		colorFormat = ColorFormat::RGB;
		break;
	case PNG_COLOR_TYPE_RGBA:
		colorFormat = ColorFormat::RGBA;
		break;
	default:
		png_destroy_read_struct(&png, &info, nullptr);
		SL_THROW_FMT(EngineException, "Unsupported PNG color type ", colorType);
	}

	auto stride = png_get_rowbytes(png, info);
	auto result = SL_NEW2(Image, { width, height, colorFormat });
	result->data.resize(stride * height);
	auto rows = png_get_rows(png, info);
	for (unsigned int i = 0; i < height; ++i)
		memcpy(result->data.data() + stride * (height - i - 1), rows[i], stride);

	png_destroy_read_struct(&png, &info, nullptr);

	return result;
}