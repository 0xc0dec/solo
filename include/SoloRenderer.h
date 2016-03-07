#pragma once

#include "SoloBase.h"


#define SL_RENDERER_RESOURCE_HANDLE(name) \
    struct name \
    { \
        int value = EmptyHandleValue; \
        \
        bool empty() \
        { \
            return value == EmptyHandleValue; \
        } \
    }; \
    \
    const name Empty##name = name();

namespace solo
{
    enum class DeviceMode;

    const int EmptyHandleValue = -1;

    SL_RENDERER_RESOURCE_HANDLE(TextureHandle)
    SL_RENDERER_RESOURCE_HANDLE(FrameBufferHandle)
    SL_RENDERER_RESOURCE_HANDLE(VertexBufferHandle)
    SL_RENDERER_RESOURCE_HANDLE(IndexBufferHandle)
    SL_RENDERER_RESOURCE_HANDLE(ProgramHandle)

    enum TextureFlags
    {
        MinFilterNearest = 1 << 0,
        MinFilterLinear = 1 << 1,
        MinFilterNearestMipmapNearest = 1 << 2,
        MinFilterLinearMipmapNearest = 1 << 3,
        MinFilterNearestMipmapLinear = 1 << 4,
        MinFilterLinearMipmapLinear = 1 << 5,
        MagFilterNearest = 1 << 6,
        MagFilterLinear = 1 << 7,
        MagFilterNearestMipmapNearest = 1 << 8,
        MagFilterLinearMipmapNearest = 1 << 9,
        MagFilterNearestMipmapLinear = 1 << 10,
        MagFilterLinearMipmapLinear = 1 << 11,
        HorizontalWrapClamp = 1 << 12,
        HorizontalWrapRepeat = 1 << 13,
        VerticalWrapClamp = 1 << 14,
        VerticalWrapRepeat = 1 << 15,
        DepthWrapClamp = 1 << 16,
        DepthWrapRepeat = 1 << 17
    };

    enum class ColorFormat
    {
        RGB,
        RGBA
    };

    enum class CubeTextureFace
    {
        Front = 0,
        Back = 1,
        Left = 2,
        Right = 3,
        Top = 4,
        Bottom = 5
    };

    class Renderer
    {
    public:
        static shared<Renderer> create(DeviceMode mode);

        SL_NONCOPYABLE(Renderer)
        virtual ~Renderer();

        virtual TextureHandle createTexture() = 0;
        virtual void destroyTexture(TextureHandle handle) = 0;
        virtual void set2DTexture(TextureHandle handle) = 0;
        virtual void set2DTexture(TextureHandle handle, int flags) = 0;
        virtual void set2DTexture(TextureHandle handle, int flags, float anisotropyLevel) = 0;
        virtual void setCubeTexture(TextureHandle handle) = 0;
        virtual void setCubeTexture(TextureHandle handle, int flags) = 0;
        virtual void setCubeTexture(TextureHandle handle, int flags, float anisotropyLevel) = 0;
        virtual void update2DTexture(TextureHandle handle, ColorFormat format, int width, int height,
            const std::vector<uint8_t>& data) = 0;
        virtual void updateCubeTexture(TextureHandle handle, CubeTextureFace face, ColorFormat format, int width, int height,
            const std::vector<uint8_t>& data) = 0;

        virtual FrameBufferHandle createFrameBuffer() = 0;
        virtual void destroyFrameBuffer(FrameBufferHandle handle) = 0;
        virtual void setFrameBuffer(FrameBufferHandle handle) = 0;
        virtual void updateFrameBuffer(FrameBufferHandle handle, const std::vector<TextureHandle> attachments) = 0;

    protected:
        Renderer();
    };
}