/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#include "SoloPngImageLoader.h"
#include "SoloFileSystem.h"
#include "SoloImage.h"
#include "SoloDevice.h"
#include <png.h>

using namespace solo;


struct PngReadContext
{
    std::vector<uint8_t>* bytes;
    size_t offset;
};


static void callback(png_structp png, png_bytep data, png_size_t length)
{
    auto context = reinterpret_cast<PngReadContext*>(png_get_io_ptr(png));
    if (!context)
        png_error(png, "Failed to read PNG");
    memcpy(data, context->bytes->data() + context->offset, length);
    context->offset += length;
}


bool PngImageLoader::isLoadable(const std::string& path) const
{
    return path.find(".png", path.size() - 5) != std::string::npos;
}


auto PngImageLoader::load(const std::string& path) -> sptr<Image>
{
    auto bytes = Device::get()->getFileSystem()->readBytes(path);
    SL_ERR_IF(bytes.size() < 8 || png_sig_cmp(&bytes[0], 0, 8) != 0, SL_FMT("Failed to recognize file '", path, "' as PNG image"));

    auto png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    auto info = png_create_info_struct(png);
    if (setjmp(png_jmpbuf(png)))
    {
        png_destroy_info_struct(png, &info);
        png_destroy_read_struct(&png, &info, nullptr);
        SL_ERR(SL_FMT("Failed to read file '", path, "' as PNG image"));
    }

    std::unique_ptr<PngReadContext> context(new PngReadContext{ &bytes, 8 });
    png_set_read_fn(png, reinterpret_cast<png_voidp>(context.get()), callback);
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
            SL_ERR(SL_FMT("Unsupported PNG color type ", colorType));
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