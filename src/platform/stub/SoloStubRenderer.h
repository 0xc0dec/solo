#pragma once

#include "SoloRenderer.h"

namespace solo
{
    class StubRenderer final : public Renderer
    {
    public:
        virtual TextureHandle createTexture() override final { return EmptyTextureHandle; }
        virtual void destroyTexture(const TextureHandle& handle) override final {}
        virtual void set2DTexture(const TextureHandle& handle) override final {}
        virtual void set2DTexture(const TextureHandle& handle, uint32_t flags) override final {}
        virtual void set2DTexture(const TextureHandle& handle, uint32_t flags, float anisotropyLevel) override final {}
        virtual void setCubeTexture(const TextureHandle& handle) override final {}
        virtual void setCubeTexture(const TextureHandle& handle, uint32_t flags) override final {}
        virtual void setCubeTexture(const TextureHandle& handle, uint32_t flags, float anisotropyLevel) override final {}
        virtual void update2DTexture(const TextureHandle& handle, ColorFormat format, uint32_t width, uint32_t height,
            const void* data) override final {}
        virtual void updateCubeTexture(const TextureHandle& handle, CubeTextureFace face, ColorFormat format,
            uint32_t width, uint32_t height, const void* data) override final {}
        virtual void generateTexture2DMipmaps(const TextureHandle& handle) override final {}
        virtual void generateCubeTextureMipmaps(const TextureHandle& handle) override final {}

        virtual FrameBufferHandle createFrameBuffer() override final { return EmptyFrameBufferHandle; }
        virtual void destroyFrameBuffer(const FrameBufferHandle& handle) override final {}
        virtual void setFrameBuffer(const FrameBufferHandle& handle) override final {}
        virtual void updateFrameBuffer(const FrameBufferHandle& handle, const std::vector<TextureHandle>& attachmentHandles) override final {}

        virtual VertexBufferHandle createVertexBuffer(const VertexBufferLayout& layout,
            const void* data, uint32_t vertexCount) override final { return EmptyVertexBufferHandle; }
        virtual void destroyVertexBuffer(const VertexBufferHandle& handle) override final {}

        virtual IndexBufferHandle createIndexBuffer(const void* data, uint32_t elementSize,
            uint32_t elementCount) override final { return EmptyIndexBufferHandle; }
        virtual void destroyIndexBuffer(const IndexBufferHandle& handle) override final {}

        virtual ProgramHandle createProgram(const char* vsSrc, const char* fsSrc) override final { return EmptyProgramHandle; }
        virtual void destroyProgram(const ProgramHandle& handle) override final {}
        virtual void setProgram(const ProgramHandle& handle) override final {}

        virtual VertexObjectHandle createVertexObject(const VertexBufferHandle* bufferHandles,
            uint32_t bufferCount, ProgramHandle programHandle) override final { return EmptyVertexObjectHandle; }
        virtual void destroyVertexObject(const VertexObjectHandle& handle) override final {}

        virtual UniformHandle createUniform(const char* name, UniformType type,
            uint32_t componentCount, ProgramHandle program) override final { return EmptyUniformHandle; }
        virtual void destroyUniform(const UniformHandle& handle) override final {}
        virtual void setUniform(const UniformHandle& handle, const void* value, uint32_t count) override final {}

        virtual void setPolygonFace(PolygonFace state) override final {}

        virtual void setBlend(bool enabled) override final {}
        virtual void setBlendFactor(BlendFactor srcFactor, BlendFactor dstFactor) override final {}

        virtual void setDepthWrite(bool enabled) override final {}
        virtual void setDepthTest(bool enabled) override final {}
        virtual void setDepthFunction(DepthFunction func) override final {}

        virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override final {}

        virtual void clear(bool color, bool depth, float r, float g, float b, float a) override final {}

        virtual void drawIndexedVertexObject(PrimitiveType primitiveType, const VertexObjectHandle& vertexObjectHandle,
            const IndexBufferHandle& indexBufferHandle) override final {}
        virtual void drawVertexObject(PrimitiveType primitiveType, const VertexObjectHandle& vertexObjectHandle, uint32_t vertexCount) override final {}

    private:
        friend class Renderer;

        StubRenderer(Device* device): Renderer(device)
        {
        }
    };
}