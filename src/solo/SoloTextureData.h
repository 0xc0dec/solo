/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

namespace solo
{
    enum class TextureFormat;
    class Device;

    class Texture2DData: public NoCopyAndMove
    {
    public:
        static auto loadFromFile(Device *device, const str &path) -> sptr<Texture2DData>;
        static auto createFromMemory(u32 width, u32 height, TextureFormat format, const vec<u8> &data) -> sptr<Texture2DData>;

		virtual ~Texture2DData() = default;

        virtual auto getSize() const -> u32 = 0;
        virtual auto getWidth() const -> u32 = 0;
        virtual auto getHeight() const -> u32 = 0;
        virtual auto getData() const -> const void* = 0;
        virtual auto getFormat() const -> TextureFormat = 0;

    protected:
		Texture2DData() = default;
    };

    class CubeTextureData: public NoCopyAndMove
    {
    public:
        static auto loadFromFaceFiles(
            Device *device,
            const str& positiveXPath, const str& negativeXPath,
			const str& positiveYPath, const str& negativeYPath,
			const str& positiveZPath, const str& negativeZPath) -> sptr<CubeTextureData>;
	
        virtual ~CubeTextureData() = default;

        virtual auto getSize() const -> u32 = 0;
        virtual auto getSize(u32 face) const -> u32 = 0; // TODO use TextureFace enum
        virtual auto getDimension() const -> u32 = 0;
        virtual auto getData(u32 face) const -> const void* = 0;
        virtual auto getFormat() const -> TextureFormat = 0;

    protected:
		CubeTextureData() = default;
    };
}