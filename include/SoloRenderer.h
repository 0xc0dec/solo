#pragma once

#include "SoloBase.h"
#include "SoloVertexFormat.h"
#include <vector>


#define SL_RENDERER_RESOURCE_HANDLE(name) \
    struct name \
    { \
        uint32_t value = EmptyHandleValue; \
        \
        bool empty() const\
        { \
            return value == EmptyHandleValue; \
        } \
    }; \
    \
    const name Empty##name = name();

namespace solo
{
    class Device;

    const uint32_t EmptyHandleValue = -1;

    SL_RENDERER_RESOURCE_HANDLE(TextureHandle)
    SL_RENDERER_RESOURCE_HANDLE(FrameBufferHandle)
    SL_RENDERER_RESOURCE_HANDLE(VertexBufferHandle)
    SL_RENDERER_RESOURCE_HANDLE(IndexBufferHandle)
    SL_RENDERER_RESOURCE_HANDLE(ProgramHandle)
    SL_RENDERER_RESOURCE_HANDLE(VertexObjectHandle)
    SL_RENDERER_RESOURCE_HANDLE(UniformHandle)

    enum class TextureFlags
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

    enum class StateFlags
    {
        CullFace = 1 << 0,
        FrontFaceCCW = 1 << 1, // if not set then CW
        DepthTest = 1 << 2,
        DepthWrite = 1 << 3,
        DepthFuncNever = 1 << 4,
        DepthFuncLess = 1 << 5,
        DepthFuncEqual = 1 << 6,
        DepthFuncLEqual = 1 << 7,
        DepthFuncGreater = 1 << 8,
        DepthFuncNotEqual = 1 << 9,
        DepthFuncGEqual = 1 << 10,
        DepthFuncAlways = 1 << 11,
    };

    enum class PrimitiveType
    {
        Triangles,
        TriangleStrip,
        Lines,
        LineStrip,
        Points
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
        static shared<Renderer> create(Device* device);

        SL_NONCOPYABLE(Renderer)
        virtual ~Renderer();

        Device* getDevice() const;

        virtual TextureHandle createTexture() = 0;
        virtual void destroyTexture(const TextureHandle& handle) = 0;
        virtual void set2DTexture(const TextureHandle& handle) = 0;
        virtual void set2DTexture(const TextureHandle& handle, uint32_t flags) = 0;
        virtual void set2DTexture(const TextureHandle& handle, uint32_t flags, float anisotropyLevel) = 0;
        virtual void setCubeTexture(const TextureHandle& handle) = 0;
        virtual void setCubeTexture(const TextureHandle& handle, uint32_t flags) = 0;
        virtual void setCubeTexture(const TextureHandle& handle, uint32_t flags, float anisotropyLevel) = 0;
        virtual void update2DTexture(const TextureHandle& handle, ColorFormat format, uint32_t width, uint32_t height, const void* data) = 0;
        virtual void updateCubeTexture(const TextureHandle& handle, CubeTextureFace face, ColorFormat format,
            uint32_t width, uint32_t height, const void* data) = 0;
        virtual void generateTexture2DMipmaps(const TextureHandle& handle) = 0;
        virtual void generateCubeTextureMipmaps(const TextureHandle& handle) = 0;

        virtual FrameBufferHandle createFrameBuffer() = 0;
        virtual void destroyFrameBuffer(const FrameBufferHandle& handle) = 0;
        virtual void setFrameBuffer(const FrameBufferHandle& handle) = 0;
        virtual void updateFrameBuffer(const FrameBufferHandle& handle, const std::vector<TextureHandle> attachmentHandles) = 0;

        virtual VertexBufferHandle createVertexBuffer(const VertexBufferLayout& layout, const void* data, uint32_t vertexCount) = 0;
        virtual void destroyVertexBuffer(const VertexBufferHandle& handle) = 0;

        virtual IndexBufferHandle createIndexBuffer(const void* data, uint32_t elementSize, uint32_t elementCount) = 0;
        virtual void destroyIndexBuffer(const IndexBufferHandle& handle) = 0;

        virtual ProgramHandle createProgram(const char* vsSrc, const char* fsSrc) = 0;
        virtual void destroyProgram(const ProgramHandle& handle) = 0;
        virtual void setProgram(const ProgramHandle& handle) = 0;

        virtual VertexObjectHandle createVertexObject(const VertexBufferHandle* bufferHandles, uint32_t bufferCount, ProgramHandle programHandle) = 0;
        virtual void destroyVertexObject(const VertexObjectHandle& handle) = 0;

        virtual UniformHandle createUniform(const char* name, UniformType type, uint32_t componentCount, ProgramHandle program) = 0;
        virtual void destroyUniform(const UniformHandle& handle) = 0;
        virtual void setUniform(const UniformHandle& handle, const void* value, uint32_t count) = 0;

        virtual void setState(uint32_t stateFlags) = 0;

        virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

        virtual void clear(bool color, bool depth, float r, float g, float b, float a) = 0;

        virtual void drawIndexedVertexObject(PrimitiveType primitiveType, const VertexObjectHandle& vertexObjectHandle,
            const IndexBufferHandle& indexBufferHandle) = 0;
        virtual void drawVertexObject(PrimitiveType primitiveType, const VertexObjectHandle& vertexObjectHandle, uint32_t vertexCount) = 0;

    protected:
        explicit Renderer(Device* device);

        Device* device;
    };

    inline Device* Renderer::getDevice() const
    {
        return device;
    }
}