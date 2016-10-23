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

#pragma once

#include "SoloCommon.h"
#include <vector>

namespace solo
{
    class Device;
    class DeviceToken;

    struct TextureFlags
    {
        static const uint32_t MinFilterNearest = 1 << 0;
        static const uint32_t MinFilterLinear = 1 << 1;
        static const uint32_t MinFilterNearestMipmapNearest = 1 << 2;
        static const uint32_t MinFilterLinearMipmapNearest = 1 << 3;
        static const uint32_t MinFilterNearestMipmapLinear = 1 << 4;
        static const uint32_t MinFilterLinearMipmapLinear = 1 << 5;
        static const uint32_t MagFilterNearest = 1 << 6;
        static const uint32_t MagFilterLinear = 1 << 7;
        static const uint32_t MagFilterNearestMipmapNearest = 1 << 8;
        static const uint32_t MagFilterLinearMipmapNearest = 1 << 9;
        static const uint32_t MagFilterNearestMipmapLinear = 1 << 10;
        static const uint32_t MagFilterLinearMipmapLinear = 1 << 11;
        static const uint32_t HorizontalWrapClamp = 1 << 12;
        static const uint32_t HorizontalWrapRepeat = 1 << 13;
        static const uint32_t VerticalWrapClamp = 1 << 14;
        static const uint32_t VerticalWrapRepeat = 1 << 15;
        static const uint32_t DepthWrapClamp = 1 << 16;
        static const uint32_t DepthWrapRepeat = 1 << 17;
    };

    enum class FaceCull
    {
        All = 0,
        CW,
        CCW
    };

    enum class PolygonMode
    {
        Triangle,
        Wireframe,
        Points
    };

    enum class DepthFunction
    {
        Never = 0,
        Less,
        Equal,
        LEqual,
        Greater,
        NotEqual,
        GEqual,
        Always
    };

    enum class BlendFactor
    {
        Zero = 0,
        One,
        SrcColor,
        OneMinusSrcColor,
        DstColor,
        OneMinusDstColor,
        SrcAlpha,
        OneMinusSrcAlpha,
        DstAlpha,
        OneMinusDstAlpha,
        ConstantAlpha,
        OneMinusConstantAlpha,
        SrcAlphaSaturate
    };

    enum class PrimitiveType
    {
        Triangles,
        TriangleStrip,
        Lines,
        LineStrip,
        Points
    };

    enum class TextureFormat
    {
        Red,
        RGB,
        RGBA,
        Alpha
    };

    enum class CubeTextureFace
    {
        Front = 0,
        Back,
        Left,
        Right,
        Top,
        Bottom
    };

    enum class UniformType
    {
        Float,
        FloatArray,
        Vector2,
        Vector2Array,
        Vector3,
        Vector3Array,
        Vector4,
        Vector4Array,
        Matrix,
        MatrixArray,
        Texture,
        TextureArray,
    };

    class Renderer
    {
    public:
        static auto create(Device* device, const DeviceToken&) -> sptr<Renderer>;

        SL_DISABLE_COPY_AND_MOVE(Renderer)
        virtual ~Renderer() {}

        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;

    protected:
        Renderer() {}
    };
}