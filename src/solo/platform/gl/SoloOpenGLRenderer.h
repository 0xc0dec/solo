/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloRenderer.h"
#include "SoloVertexFormat.h"
#include <GL/glew.h>
#include <vector>
#include <unordered_map>

namespace solo
{
    class Device;

    namespace gl
    {
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

        constexpr uint32_t EmptyHandle = std::numeric_limits<uint32_t>::max();

        // TODO switch to submission-based rendering instead of immediate
        class Renderer final : public solo::Renderer
        {
        public:
            explicit Renderer(Device *device);
            ~Renderer();

            void addRenderCommand(const solo::RenderCommand &cmd) override final;

            auto createTexture() -> uint32_t;
            void destroyTexture(uint32_t handle);
            void setRectTexture(uint32_t handle);
            void setRectTexture(uint32_t handle, uint32_t flags);
            void setRectTexture(uint32_t handle, uint32_t flags, float anisotropyLevel);
            void setCubeTexture(uint32_t handle);
            void setCubeTexture(uint32_t handle, uint32_t flags);
            void setCubeTexture(uint32_t handle, uint32_t flags, float anisotropyLevel);
            void updateRectTexture(uint32_t handle, TextureFormat format, uint32_t width, uint32_t height, const void *data);
            void updateCubeTexture(uint32_t handle, CubeTextureFace face, TextureFormat format, uint32_t width, uint32_t height, const void *data);
            void generateRectTextureMipmaps(uint32_t handle);
            void generateCubeTextureMipmaps(uint32_t handle);

            auto createFrameBuffer() -> uint32_t;
            void destroyFrameBuffer(uint32_t handle);
            void setFrameBuffer(uint32_t handle);
            void updateFrameBuffer(uint32_t handle, const std::vector<uint32_t> &attachmentHandles);

            auto createVertexBuffer(const VertexBufferLayout &layout, const void *data, uint32_t vertexCount) -> uint32_t;
            auto createDynamicVertexBuffer(const VertexBufferLayout &layout, const void *data, uint32_t vertexCount) -> uint32_t;
            void updateDynamicVertexBuffer(uint32_t handle, const void *data, uint32_t offset, uint32_t vertexCount);
            void destroyVertexBuffer(uint32_t handle);

            auto createIndexBuffer(const void *data, uint32_t elementSize, uint32_t elementCount) -> uint32_t;
            void destroyIndexBuffer(uint32_t handle);

            auto createProgram(const void *vsSrc, uint32_t vsSrcLength, const void *fsSrc, uint32_t fsSrcLength) -> uint32_t;
            void destroyProgram(uint32_t handle);
            void setProgram(uint32_t handle);

            auto createVertexArray(const uint32_t *bufferHandles, uint32_t bufferCount) -> uint32_t;
            void destroyVertexArray(uint32_t handle);

            auto createUniform(const char *name, UniformType type, uint32_t programHandle) -> uint32_t;
            void destroyUniform(uint32_t handle);
            void setUniform(uint32_t handle, const void *value, uint32_t count);

            void setFaceCull(FaceCull face);

            void setPolygonMode(PolygonMode mode);

            void setBlend(bool enabled);
            void setBlendFactor(BlendFactor srcFactor, BlendFactor dstFactor);

            void setDepthWrite(bool enabled);
            void setDepthTest(bool enabled);
            void setDepthFunction(DepthFunction func);

            void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

            void clear(bool color, bool depth, float r, float g, float b, float a);

            void drawIndexed(PrimitiveType primitiveType, uint32_t vertexArrayHandle, uint32_t indexBufferHandle);
            void draw(PrimitiveType primitiveType, uint32_t vertexArrayHandle, uint32_t vertexCount);

        protected:
            void beginFrame() override final;
            void endFrame() override final;

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

            struct Uniform
            {
                UniformType type = UniformType::Float;
                GLint location = 0;
                GLint index = 0;
            };

            // Counters for assigning unique resource ids.
            // Released ids do not get reused, so this effectively limits the total
            // number of resource allocations during the program lifetime. That's how it is for now.
            uint32_t textureCounter = 0;
            uint32_t frameBufferCounter = 0;
            uint32_t vertexBufferCounter = 0;
            uint32_t indexBufferCounter = 0;
            uint32_t programCounter = 0;
            uint32_t vertexArrayCounter = 0;
            uint32_t uniformCounter = 0;

            std::unordered_map<uint32_t, Texture> textures;
            std::unordered_map<uint32_t, FrameBuffer> frameBuffers;
            std::unordered_map<uint32_t, VertexBuffer> vertexBuffers;
            std::unordered_map<uint32_t, IndexBuffer> indexBuffers;
            std::unordered_map<uint32_t, GLuint> programs;
            std::unordered_map<uint32_t, GLuint> vertexArrays;
            std::unordered_map<uint32_t, Uniform> uniforms;

            std::vector<RenderCommand> renderCommands;

            void bindFrameBuffer(uint32_t handle);
            void bindTexture(GLenum target, uint32_t handle);
            void bindVertexBuffer(uint32_t handle);
            void bindIndexBuffer(uint32_t handle);
            void bindVertexArray(uint32_t handle);
            void setTexture(GLenum target, uint32_t handle, uint32_t flags);
            void validateFrameBufferAttachments(const std::vector<uint32_t> &attachments);
            auto createVertexBuffer(bool dynamic, const VertexBufferLayout &layout, const void *data, uint32_t vertexCount) -> uint32_t;
        };
    }
}

#endif
