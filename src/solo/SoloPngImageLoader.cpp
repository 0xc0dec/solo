/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloPngImageLoader.h"
#include "SoloFileSystem.h"
#include "SoloImage.h"
#include "SoloTexture.h"
#include "SoloDevice.h"
#include <png.h>

using namespace solo;


struct ReadContext
{
    uint8_t *data;
    uint32_t offset;
};


static void callback(png_structp png, png_bytep data, png_size_t length)
{
    auto context = reinterpret_cast<ReadContext*>(png_get_io_ptr(png));
    memcpy(data, context->data + context->offset, length);
    context->offset += length;
}


PngImageLoader::PngImageLoader(Device *device):
    fs(device->getFileSystem())
{
}


bool PngImageLoader::isLoadable(const std::string &path) const
{
    return path.find(".png", path.size() - 5) != std::string::npos;
}


auto PngImageLoader::load(const std::string &path) -> sptr<Image>
{
    auto bytes = fs->readBytes(path);
    SL_PANIC_IF(bytes.size() < 8 || png_sig_cmp(&bytes[0], 0, 8) != 0, SL_FMT("Failed to recognize file '", path, "' as PNG image"));

    auto png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    auto info = png_create_info_struct(png);
    if (setjmp(png_jmpbuf(png)))
    {
        png_destroy_info_struct(png, &info);
        png_destroy_read_struct(&png, &info, nullptr);
        SL_PANIC(SL_FMT("Failed to read file '", path, "' as PNG image"));
    }

    ReadContext context{const_cast<uint8_t*>(bytes.data()), 8};
    png_set_read_fn(png, reinterpret_cast<png_voidp>(&context), callback);
    png_set_sig_bytes(png, 8);
    png_read_png(png, info, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, nullptr);

    auto width = png_get_image_width(png, info);
    auto height = png_get_image_height(png, info);
    auto colorType = png_get_color_type(png, info);

    TextureFormat colorFormat;
    switch (colorType)
    {
        case PNG_COLOR_TYPE_RGB:
            colorFormat = TextureFormat::RGB;
            break;
        case PNG_COLOR_TYPE_RGBA:
            colorFormat = TextureFormat::RGBA;
            break;
        default:
            png_destroy_info_struct(png, &info);
            png_destroy_read_struct(&png, &info, nullptr);
            SL_PANIC(SL_FMT("Unsupported PNG color type ", colorType));
    }

    auto stride = png_get_rowbytes(png, info);
    auto result = std::make_shared<Image>();
    result->width = static_cast<uint32_t>(width);
    result->height = static_cast<uint32_t>(height);
    result->format = colorFormat;
    result->data.resize(stride * height);

    auto rows = png_get_rows(png, info);
    for (uint32_t i = 0; i < height; ++i)
        memcpy(result->data.data() + stride * (height - i - 1), rows[i], stride);

    png_destroy_info_struct(png, &info);
    png_destroy_read_struct(&png, &info, nullptr);

    return result;
}