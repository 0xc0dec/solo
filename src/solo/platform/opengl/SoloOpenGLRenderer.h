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
#include "SoloVertexFormat.h"
#include <vector>

#ifdef SL_OPENGL_RENDERER

#include <GL/glew.h>

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

        auto createTexture() -> TextureHandle;
        void destroyTexture(const TextureHandle& handle);
        void set2DTexture(const TextureHandle& handle);
        void set2DTexture(const TextureHandle& handle, uint32_t flags);
        void set2DTexture(const TextureHandle& handle, uint32_t flags, float anisotropyLevel);
        void setCubeTexture(const TextureHandle& handle);
        void setCubeTexture(const TextureHandle& handle, uint32_t flags);
        void setCubeTexture(const TextureHandle& handle, uint32_t flags, float anisotropyLevel);
        void update2DTexture(const TextureHandle& handle, TextureFormat format, uint32_t width, uint32_t height, const void* data);
        void updateCubeTexture(const TextureHandle& handle, CubeTextureFace face, TextureFormat format,
            uint32_t width, uint32_t height, const void* data);
        void generateRectTextureMipmaps(const TextureHandle& handle);
        void generateCubeTextureMipmaps(const TextureHandle& handle);

        auto createFrameBuffer() -> FrameBufferHandle;
        void destroyFrameBuffer(const FrameBufferHandle& handle);
        void setFrameBuffer(const FrameBufferHandle& handle);
        void updateFrameBuffer(const FrameBufferHandle& handle, const std::vector<TextureHandle>& attachmentHandles);

        auto createVertexBuffer(const VertexBufferLayout& layout, const void* data, uint32_t vertexCount) -> VertexBufferHandle;
        auto createDynamicVertexBuffer(const VertexBufferLayout& layout, const void* data, uint32_t vertexCount) -> VertexBufferHandle;
        void updateDynamicVertexBuffer(const VertexBufferHandle& handle, const void* data, uint32_t offset, uint32_t vertexCount);
        void destroyVertexBuffer(const VertexBufferHandle& handle);

        auto createIndexBuffer(const void* data, uint32_t elementSize, uint32_t elementCount) -> IndexBufferHandle;
        void destroyIndexBuffer(const IndexBufferHandle& handle);

        auto createProgram(const char* vsSrc, const char* fsSrc) -> ProgramHandle;
        void destroyProgram(const ProgramHandle& handle);
        void setProgram(const ProgramHandle& handle);

        auto createVertexProgramBinding(const VertexBufferHandle* bufferHandles, uint32_t bufferCount, ProgramHandle programHandle)
            -> VertexProgramBindingHandle;
        void destroyVertexProgramBinding(const VertexProgramBindingHandle& handle);

        auto createUniform(const char* name, UniformType type, ProgramHandle programHandle) -> UniformHandle;
        void destroyUniform(const UniformHandle& handle);
        void setUniform(const UniformHandle& handle, const void* value, uint32_t count);

        void setFaceCull(FaceCull face);

        void setPolygonMode(PolygonMode mode);

        void setBlend(bool enabled);
        void setBlendFactor(BlendFactor srcFactor, BlendFactor dstFactor);

        void setDepthWrite(bool enabled);
        void setDepthTest(bool enabled);
        void setDepthFunction(DepthFunction func);

        void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

        void clear(bool color, bool depth, float r, float g, float b, float a);

        void drawIndexed(PrimitiveType primitiveType, const VertexProgramBindingHandle& bindingHandle, const IndexBufferHandle& indexBufferHandle);
        void draw(PrimitiveType primitiveType, const VertexProgramBindingHandle& bindingHandle, uint32_t vertexCount);

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
