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
#include "SoloRenderer.h"
#include "SoloResourcePool.h"
#include <GL/glew.h>
#include <vector>

#ifdef SL_OPENGL_RENDERER

namespace solo
{
    class Device;

    class OpenGLRenderer final: public Renderer
    {
    public:
        explicit OpenGLRenderer(Device* device);
        ~OpenGLRenderer();

        void beginFrame() override final {}
        void endFrame() override final {}

        auto createTexture() -> TextureHandle override final;
        void destroyTexture(const TextureHandle& handle) override final;
        void set2DTexture(const TextureHandle& handle) override final;
        void set2DTexture(const TextureHandle& handle, uint32_t flags) override final;
        void set2DTexture(const TextureHandle& handle, uint32_t flags, float anisotropyLevel) override final;
        void setCubeTexture(const TextureHandle& handle) override final;
        void setCubeTexture(const TextureHandle& handle, uint32_t flags) override final;
        void setCubeTexture(const TextureHandle& handle, uint32_t flags, float anisotropyLevel) override final;
        void update2DTexture(const TextureHandle& handle, TextureFormat format, uint32_t width, uint32_t height,
            const void* data) override final;
        void updateCubeTexture(const TextureHandle& handle, CubeTextureFace face, TextureFormat format,
            uint32_t width, uint32_t height, const void* data) override final;
        void generateRectTextureMipmaps(const TextureHandle& handle) override final;
        void generateCubeTextureMipmaps(const TextureHandle& handle) override final;

        auto createFrameBuffer() -> FrameBufferHandle override final;
        void destroyFrameBuffer(const FrameBufferHandle& handle) override final;
        void setFrameBuffer(const FrameBufferHandle& handle) override final;
        void updateFrameBuffer(const FrameBufferHandle& handle, const std::vector<TextureHandle>& attachmentHandles) override final;

        auto createVertexBuffer(const VertexBufferLayout& layout, const void* data, uint32_t vertexCount)
            -> VertexBufferHandle override final;
        auto createDynamicVertexBuffer(const VertexBufferLayout& layout, const void* data, uint32_t vertexCount)
            -> VertexBufferHandle override final;
        void updateDynamicVertexBuffer(const VertexBufferHandle& handle, const void* data, uint32_t offset, uint32_t vertexCount) override final;
        void destroyVertexBuffer(const VertexBufferHandle& handle) override final;

        auto createIndexBuffer(const void* data, uint32_t elementSize, uint32_t elementCount) -> IndexBufferHandle override final;
        void destroyIndexBuffer(const IndexBufferHandle& handle) override final;

        auto createProgram(const char* vsSrc, const char* fsSrc) -> ProgramHandle override final;
        void destroyProgram(const ProgramHandle& handle) override final;
        void setProgram(const ProgramHandle& handle) override final;

        auto createVertexProgramBinding(const VertexBufferHandle* bufferHandles, uint32_t bufferCount, ProgramHandle programHandle)
            -> VertexProgramBindingHandle override final;
        void destroyVertexProgramBinding(const VertexProgramBindingHandle& handle) override final;

        auto createUniform(const char* name, UniformType type, ProgramHandle programHandle) -> UniformHandle override final;
        void destroyUniform(const UniformHandle& handle) override final;
        void setUniform(const UniformHandle& handle, const void* value, uint32_t count) override final;

        void setFaceCull(FaceCull face);

        void setPolygonMode(PolygonMode mode);

        void setBlend(bool enabled);
        void setBlendFactor(BlendFactor srcFactor, BlendFactor dstFactor);

        void setDepthWrite(bool enabled);
        void setDepthTest(bool enabled);
        void setDepthFunction(DepthFunction func);

        void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override final;

        void clear(bool color, bool depth, float r, float g, float b, float a) override final;

        void drawIndexed(PrimitiveType primitiveType, const VertexProgramBindingHandle& bindingHandle,
            const IndexBufferHandle& indexBufferHandle) override final;
        void draw(PrimitiveType primitiveType, const VertexProgramBindingHandle& bindingHandle, uint32_t vertexCount) override final;

    private:
        struct FrameBuffer
        {
            GLuint rawHandle = 0;
            GLuint depthBufferHandle = 0;
            uint32_t attachmentCount = 0;
        };

        struct Texture
        {
            GLuint rawHandle = 0;
            uint32_t width = 0;
            uint32_t height = 0;
        };

        struct VertexBuffer
        {
            VertexBufferLayout layout;
            GLuint rawHandle = 0;
            uint32_t vertexCount = 0;
            bool dynamic = false;
        };

        struct IndexBuffer
        {
            GLuint rawHandle = 0;
            uint32_t elementCount = 0;
        };

        struct Program
        {
            GLuint rawHandle = 0;
        };

        struct VertexProgramBinding
        {
            GLuint rawHandle = 0;
        };

        struct Uniform
        {
            UniformType type = UniformType::Float;
            GLint location = 0;
            GLint index = 0;
        };

        void bindFrameBuffer(const FrameBufferHandle& handle);
        void bindTexture(GLenum target, const TextureHandle& handle);
        void bindVertexBuffer(const VertexBufferHandle& handle);
        void bindIndexBuffer(const IndexBufferHandle& handle);
        void bindVertexProgramBinding(const VertexProgramBindingHandle& handle);
        void setTexture(GLenum target, const TextureHandle& handle, uint32_t flags);
        void validateFrameBufferAttachments(const std::vector<TextureHandle>& attachments);
        auto createVertexBuffer(bool dynamic, const VertexBufferLayout& layout, const void* data, uint32_t vertexCount) -> VertexBufferHandle;

        ResourcePool<Texture, SL_MAX_TEXTURES> textures;
        ResourcePool<FrameBuffer, SL_MAX_FRAME_BUFFERS> frameBuffers;
        ResourcePool<VertexBuffer, SL_MAX_VERTEX_BUFFERS> vertexBuffers;
        ResourcePool<IndexBuffer, SL_MAX_INDEX_BUFFERS> indexBuffers;
        ResourcePool<Program, SL_MAX_PROGRAMS> programs;
        ResourcePool<VertexProgramBinding, SL_MAX_VERTEX_OBJECTS> vertexProgramBindings;
        ResourcePool<Uniform, SL_MAX_UNIFORMS> uniforms;
    };
}

#else
#   error OpenGL renderer is not supported on this platform
#endif
