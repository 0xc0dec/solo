#pragma once

#include "SoloRenderer.h"

namespace solo
{
    class StubRenderer final: public Renderer
    {
    public:
        virtual TextureHandle createTexture() override final { return EmptyTextureHandle; }
        virtual void destroyTexture(TextureHandle handle) override final {}
        virtual void set2DTexture(TextureHandle handle) override final {}
        virtual void set2DTexture(TextureHandle handle, int flags) override final {}
        virtual void set2DTexture(TextureHandle handle, int flags, float anisotropyLevel) override final {}
        virtual void setCubeTexture(TextureHandle handle) override final {}
        virtual void setCubeTexture(TextureHandle handle, int flags) override final {}
        virtual void setCubeTexture(TextureHandle handle, int flags, float anisotropyLevel) override final {}
        virtual void update2DTexture(TextureHandle handle, ColorFormat format, int width, int height,
            const std::vector<uint8_t>& data) override final {}
        virtual void updateCubeTexture(TextureHandle handle, CubeTextureFace face, ColorFormat format, int width, int height,
            const std::vector<uint8_t>& data) override final {}

        virtual FrameBufferHandle createFrameBuffer() override final { return EmptyFrameBufferHandle; }
        virtual void destroyFrameBuffer(FrameBufferHandle handle) override final {}
        virtual void setFrameBuffer(FrameBufferHandle handle) override final {}
        virtual void updateFrameBuffer(FrameBufferHandle handle, const std::vector<TextureHandle> attachmentHandles) override final {}

        virtual VertexBufferHandle createVertexBuffer(const VertexBufferLayout& layout,
            const void* data, int vertexCount) override final { return EmptyVertexBufferHandle; }
        virtual void destroyVertexBuffer(VertexBufferHandle handle) override final {}

        virtual IndexBufferHandle createIndexBuffer(const void* data,
            int elementSize, int elementCount) override final { return EmptyIndexBufferHandle; }
        virtual void destroyIndexBuffer(IndexBufferHandle handle) override final {}

        virtual ProgramHandle createProgram(const char* vsSrc, const char* fsSrc) override final { return EmptyProgramHandle; }
        virtual void destroyProgram(ProgramHandle handle) override final {}
        virtual void setProgram(ProgramHandle handle) override final {}

        virtual VertexObjectHandle createVertexObject(const VertexBufferHandle* bufferHandles,
            int bufferCount, ProgramHandle programHandle) override final { return EmptyVertexObjectHandle; }
        virtual void destroyVertexObject(VertexObjectHandle handle) override final {}

        virtual UniformHandle createUniform(const char* name, UniformType type, int componentCount,
            ProgramHandle program) override final { return EmptyUniformHandle; }
        virtual void destroyUniform(const UniformHandle& handle) override final {}
        virtual void setUniform(const UniformHandle& handle, const void* value, int count) override final {}

        virtual void setState(int stateFlags) override final {}

        virtual void setViewport(int x, int y, int width, int height) override final {}

        virtual void clear(bool color, bool depth, float r, float g, float b, float a) override final {}

        virtual void drawIndexedVertexObject(PrimitiveType primitiveType, const VertexObjectHandle& vertexObjectHandle,
            const IndexBufferHandle& indexBufferHandle) override final {}
        virtual void drawVertexObject(PrimitiveType primitiveType, const VertexObjectHandle& vertexObjectHandle, int vertexCount) override final {}

    private:
        friend class Renderer;

        StubRenderer(Device* device): Renderer(device)
        {
        }
    };
}