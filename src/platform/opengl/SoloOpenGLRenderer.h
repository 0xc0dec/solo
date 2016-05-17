#pragma once

#include "SoloBase.h"
#include "SoloRenderer.h"
#include "SoloResourcePool.h"
#include <GL/glew.h>
#include <vector>


namespace solo
{
    class OpenGLRenderer final: public Renderer
    {
    public:
        OpenGLRenderer() {}
        ~OpenGLRenderer();

        virtual auto createTexture() -> TextureHandle override final;
        virtual void destroyTexture(const TextureHandle& handle) override final;
        virtual void set2DTexture(const TextureHandle& handle) override final;
        virtual void set2DTexture(const TextureHandle& handle, uint32_t flags) override final;
        virtual void set2DTexture(const TextureHandle& handle, uint32_t flags, float anisotropyLevel) override final;
        virtual void setCubeTexture(const TextureHandle& handle) override final;
        virtual void setCubeTexture(const TextureHandle& handle, uint32_t flags) override final;
        virtual void setCubeTexture(const TextureHandle& handle, uint32_t flags, float anisotropyLevel) override final;
        virtual void update2DTexture(const TextureHandle& handle, TextureFormat format, uint32_t width, uint32_t height,
            const void* data) override final;
        virtual void updateCubeTexture(const TextureHandle& handle, CubeTextureFace face, TextureFormat format,
            uint32_t width, uint32_t height, const void* data) override final;
        virtual void generateRectTextureMipmaps(const TextureHandle& handle) override final;
        virtual void generateCubeTextureMipmaps(const TextureHandle& handle) override final;

        virtual auto createFrameBuffer() -> FrameBufferHandle override final;
        virtual void destroyFrameBuffer(const FrameBufferHandle& handle) override final;
        virtual void setFrameBuffer(const FrameBufferHandle& handle) override final;
        virtual void updateFrameBuffer(const FrameBufferHandle& handle, const std::vector<TextureHandle>& attachmentHandles) override final;

        virtual auto createVertexBuffer(const VertexBufferLayout& layout, const void* data, uint32_t vertexCount)
            -> VertexBufferHandle override final;
        virtual auto createDynamicVertexBuffer(const VertexBufferLayout& layout, const void* data, uint32_t vertexCount)
            -> VertexBufferHandle override final;
        virtual void updateDynamicVertexBuffer(const VertexBufferHandle& handle, const void* data, uint32_t offset, uint32_t vertexCount) override final;
        virtual void destroyVertexBuffer(const VertexBufferHandle& handle) override final;

        virtual auto createIndexBuffer(const void* data, uint32_t elementSize, uint32_t elementCount) -> IndexBufferHandle override final;
        virtual void destroyIndexBuffer(const IndexBufferHandle& handle) override final;

        virtual auto createProgram(const char* vsSrc, const char* fsSrc) -> ProgramHandle override final;
        virtual void destroyProgram(const ProgramHandle& handle) override final;
        virtual void setProgram(const ProgramHandle& handle) override final;

        virtual auto createVertexObject(const VertexBufferHandle* bufferHandles, uint32_t bufferCount, ProgramHandle programHandle)
            -> VertexObjectHandle override final;
        virtual void destroyVertexObject(const VertexObjectHandle& handle) override final;

        virtual auto createUniform(const char* name, UniformType type, ProgramHandle program) -> UniformHandle override final;
        virtual void destroyUniform(const UniformHandle& handle) override final;
        virtual void setUniform(const UniformHandle& handle, const void* value, uint32_t count) override final;

        virtual void setFaceCull(FaceCull face) override final;

        virtual void setBlend(bool enabled) override final;
        virtual void setBlendFactor(BlendFactor srcFactor, BlendFactor dstFactor) override final;

        virtual void setDepthWrite(bool enabled) override final;
        virtual void setDepthTest(bool enabled) override final;
        virtual void setDepthFunction(DepthFunction func) override final;

        virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override final;

        virtual void clear(bool color, bool depth, float r, float g, float b, float a) override final;

        virtual void drawIndexedVertexObject(PrimitiveType primitiveType, const VertexObjectHandle& vertexObjectHandle,
            const IndexBufferHandle& indexBufferHandle) override final;
        virtual void drawVertexObject(PrimitiveType primitiveType, const VertexObjectHandle& vertexObjectHandle, uint32_t vertexCount) override final;

    private:
        struct FrameBufferData
        {
            GLuint rawHandle = 0;
            GLuint depthBufferHandle = 0;
            uint32_t attachmentCount = 0;
        };

        struct TextureData
        {
            GLuint rawHandle = 0;
            uint32_t width = 0;
            uint32_t height = 0;
        };

        struct VertexBufferData
        {
            VertexBufferLayout layout;
            GLuint rawHandle = 0;
            uint32_t vertexCount = 0;
            bool dynamic = false;
        };

        struct IndexBufferData
        {
            GLuint rawHandle = 0;
            uint32_t elementCount = 0;
        };

        struct ProgramData
        {
            GLuint rawHandle = 0;
        };

        struct VertexObjectData
        {
            GLuint rawHandle = 0;
        };

        struct UniformData
        {
            UniformType type = UniformType::Float;
            GLint location = 0;
            GLint index = 0;
        };

        void bindFrameBuffer(const FrameBufferHandle& handle);
        void bindTexture(GLenum target, const TextureHandle& handle);
        void bindVertexBuffer(const VertexBufferHandle& handle);
        void bindIndexBuffer(const IndexBufferHandle& handle);
        void bindVertexObject(const VertexObjectHandle& handle);
        void setTexture(GLenum target, const TextureHandle& handle, uint32_t flags);
        void validateFrameBufferAttachments(const std::vector<TextureHandle>& attachments);
        auto createVertexBuffer(bool dynamic, const VertexBufferLayout& layout, const void* data, uint32_t vertexCount) -> VertexBufferHandle;

        ResourcePool<TextureData, SL_MAX_TEXTURES> textures;
        ResourcePool<FrameBufferData, SL_MAX_FRAME_BUFFERS> frameBuffers;
        ResourcePool<VertexBufferData, SL_MAX_VERTEX_BUFFERS> vertexBuffers;
        ResourcePool<IndexBufferData, SL_MAX_INDEX_BUFFERS> indexBuffers;
        ResourcePool<ProgramData, SL_MAX_PROGRAMS> programs;
        ResourcePool<VertexObjectData, SL_MAX_VERTEX_OBJECTS> vertexObjects;
        ResourcePool<UniformData, SL_MAX_UNIFORMS> uniforms;
    };
}