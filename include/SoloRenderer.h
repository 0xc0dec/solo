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
    class DeviceToken;

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

    enum class PolygonFace
    {
        All = 0,
        CW,
        CCW
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

        SL_NONCOPYABLE(Renderer)
        virtual ~Renderer() {}

        virtual auto createTexture() -> TextureHandle = 0;
        virtual void destroyTexture(const TextureHandle& handle) = 0;
        virtual void set2DTexture(const TextureHandle& handle) = 0;
        virtual void set2DTexture(const TextureHandle& handle, uint32_t flags) = 0;
        virtual void set2DTexture(const TextureHandle& handle, uint32_t flags, float anisotropyLevel) = 0;
        virtual void setCubeTexture(const TextureHandle& handle) = 0;
        virtual void setCubeTexture(const TextureHandle& handle, uint32_t flags) = 0;
        virtual void setCubeTexture(const TextureHandle& handle, uint32_t flags, float anisotropyLevel) = 0;
        virtual void update2DTexture(const TextureHandle& handle, TextureFormat format, uint32_t width, uint32_t height, const void* data) = 0;
        virtual void updateCubeTexture(const TextureHandle& handle, CubeTextureFace face, TextureFormat format,
            uint32_t width, uint32_t height, const void* data) = 0;
        virtual void generateRectTextureMipmaps(const TextureHandle& handle) = 0;
        virtual void generateCubeTextureMipmaps(const TextureHandle& handle) = 0;

        virtual auto createFrameBuffer() -> FrameBufferHandle = 0;
        virtual void destroyFrameBuffer(const FrameBufferHandle& handle) = 0;
        virtual void setFrameBuffer(const FrameBufferHandle& handle) = 0;
        virtual void updateFrameBuffer(const FrameBufferHandle& handle, const std::vector<TextureHandle>& attachmentHandles) = 0;

        virtual auto createVertexBuffer(const VertexBufferLayout& layout, const void* data, uint32_t vertexCount) -> VertexBufferHandle = 0;
        virtual auto createDynamicVertexBuffer(const VertexBufferLayout& layout, const void* data, uint32_t vertexCount) -> VertexBufferHandle = 0;
        virtual void updateDynamicVertexBuffer(const VertexBufferHandle& handle, const void* data, uint32_t offset, uint32_t vertexCount) = 0;
        virtual void destroyVertexBuffer(const VertexBufferHandle& handle) = 0;

        virtual auto createIndexBuffer(const void* data, uint32_t elementSize, uint32_t elementCount) -> IndexBufferHandle = 0;
        virtual void destroyIndexBuffer(const IndexBufferHandle& handle) = 0;

        virtual auto createProgram(const char* vsSrc, const char* fsSrc) -> ProgramHandle = 0;
        virtual void destroyProgram(const ProgramHandle& handle) = 0;
        virtual void setProgram(const ProgramHandle& handle) = 0;

        virtual auto createVertexObject(const VertexBufferHandle* bufferHandles, uint32_t bufferCount, ProgramHandle programHandle) -> VertexObjectHandle = 0;
        virtual void destroyVertexObject(const VertexObjectHandle& handle) = 0;

        virtual auto createUniform(const char* name, UniformType type, ProgramHandle program) -> UniformHandle = 0;
        virtual void destroyUniform(const UniformHandle& handle) = 0;
        virtual void setUniform(const UniformHandle& handle, const void* value, uint32_t count) = 0;

        virtual void setPolygonFace(PolygonFace face) = 0;

        virtual void setBlend(bool enabled) = 0;
        virtual void setBlendFactor(BlendFactor srcFactor, BlendFactor dstFactor) = 0;

        virtual void setDepthWrite(bool enabled) = 0;
        virtual void setDepthTest(bool enabled) = 0;
        virtual void setDepthFunction(DepthFunction func) = 0;

        virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

        virtual void clear(bool color, bool depth, float r, float g, float b, float a) = 0;

        virtual void drawIndexedVertexObject(PrimitiveType primitiveType, const VertexObjectHandle& vertexObjectHandle,
            const IndexBufferHandle& indexBufferHandle) = 0;
        virtual void drawVertexObject(PrimitiveType primitiveType, const VertexObjectHandle& vertexObjectHandle, uint32_t vertexCount) = 0;

    protected:
        Renderer() {}
    };
}