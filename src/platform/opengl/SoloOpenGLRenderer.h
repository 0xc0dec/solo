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
        ~OpenGLRenderer();

        virtual TextureHandle createTexture() override final;
        virtual void destroyTexture(const TextureHandle& handle) override final;
        virtual void set2DTexture(const TextureHandle& handle) override final;
        virtual void set2DTexture(const TextureHandle& handle, int flags) override final;
        virtual void set2DTexture(const TextureHandle& handle, int flags, float anisotropyLevel) override final;
        virtual void setCubeTexture(const TextureHandle& handle) override final;
        virtual void setCubeTexture(const TextureHandle& handle, int flags) override final;
        virtual void setCubeTexture(const TextureHandle& handle, int flags, float anisotropyLevel) override final;
        virtual void update2DTexture(const TextureHandle& handle, ColorFormat format, int width, int height,
            const void* data) override final;
        virtual void updateCubeTexture(const TextureHandle& handle, CubeTextureFace face, ColorFormat format,
            int width, int height, const void* data) override final;
        virtual void generateTexture2DMipmaps(const TextureHandle& handle) override final;
        virtual void generateCubeTextureMipmaps(const TextureHandle& handle) override final;

        virtual FrameBufferHandle createFrameBuffer() override final;
        virtual void destroyFrameBuffer(const FrameBufferHandle& handle) override final;
        virtual void setFrameBuffer(const FrameBufferHandle& handle) override final;
        virtual void updateFrameBuffer(const FrameBufferHandle& handle, const std::vector<TextureHandle> attachmentHandles) override final;

        virtual VertexBufferHandle createVertexBuffer(const VertexBufferLayout& layout, const void* data, int vertexCount) override final;
        virtual void destroyVertexBuffer(const VertexBufferHandle& handle) override final;

        virtual IndexBufferHandle createIndexBuffer(const void* data, int elementSize, int elementCount) override final;
        virtual void destroyIndexBuffer(const IndexBufferHandle& handle) override final;

        virtual ProgramHandle createProgram(const char* vsSrc, const char* fsSrc) override final;
        virtual void destroyProgram(const ProgramHandle& handle) override final;
        virtual void setProgram(const ProgramHandle& handle) override final;

        virtual VertexObjectHandle createVertexObject(const VertexBufferHandle* bufferHandles, int bufferCount, ProgramHandle programHandle) override final;
        virtual void destroyVertexObject(const VertexObjectHandle& handle) override final;

        virtual UniformHandle createUniform(const char* name, UniformType type, int componentCount, ProgramHandle program) override final;
        virtual void destroyUniform(const UniformHandle& handle) override final;
        virtual void setUniform(const UniformHandle& handle, const void* value, int count) override final;

        virtual void setState(int stateFlags) override final;

        virtual void setViewport(int x, int y, int width, int height) override final;

        virtual void clear(bool color, bool depth, float r, float g, float b, float a) override final;

        virtual void drawIndexedVertexObject(PrimitiveType primitiveType, const VertexObjectHandle& vertexObjectHandle,
            const IndexBufferHandle& indexBufferHandle) override final;
        virtual void drawVertexObject(PrimitiveType primitiveType, const VertexObjectHandle& vertexObjectHandle, int vertexCount) override final;

    private:
        friend class Renderer;

        struct FrameBufferData
        {
            GLuint rawHandle = 0;
            GLuint depthBufferHandle = 0;
            int attachmentCount = 0;
        };

        struct TextureData
        {
            GLuint rawHandle = 0;
            int width = 0;
            int height = 0;
        };

        struct VertexBufferData
        {
            GLuint rawHandle = 0;
            VertexBufferLayout layout;
            int vertexCount = 0;
        };

        struct IndexBufferData
        {
            GLuint rawHandle = 0;
            int elementCount = 0;
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
            int componentCount = 0;
        };

        explicit OpenGLRenderer(Device* device);

        void bindFrameBuffer(const FrameBufferHandle& handle);
        void bindTexture(GLenum target, const TextureHandle& handle);
        void bindVertexBuffer(const VertexBufferHandle& handle);
        void bindIndexBuffer(const IndexBufferHandle& handle);
        void bindVertexObject(const VertexObjectHandle& handle);
        void setTexture(GLenum target, const TextureHandle& handle, int flags);
        void validateFrameBufferAttachments(const std::vector<TextureHandle> attachments);

        ResourcePool<TextureData, SL_MAX_TEXTURES> textures;
        ResourcePool<FrameBufferData, SL_MAX_FRAME_BUFFERS> frameBuffers;
        ResourcePool<VertexBufferData, SL_MAX_VERTEX_BUFFERS> vertexBuffers;
        ResourcePool<IndexBufferData, SL_MAX_INDEX_BUFFERS> indexBuffers;
        ResourcePool<ProgramData, SL_MAX_PROGRAMS> programs;
        ResourcePool<VertexObjectData, SL_MAX_VERTEX_OBJECTS> vertexObjects;
        ResourcePool<UniformData, SL_MAX_UNIFORMS> uniforms;
    };
}