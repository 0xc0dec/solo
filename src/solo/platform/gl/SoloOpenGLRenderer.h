/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloRenderer.h"
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

        class Renderer final : public solo::Renderer
        {
        public:
            explicit Renderer(Device *device);
            ~Renderer();

            void addRenderCommand(const RenderCommand &cmd) override final;

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

            void setFaceCull(FaceCull face);
            void setPolygonMode(PolygonMode mode);
            void setBlend(bool enabled);
            void setBlendFactor(BlendFactor srcFactor, BlendFactor dstFactor);
            void setDepthWrite(bool enabled);
            void setDepthTest(bool enabled);
            void setDepthFunction(DepthFunction func);
            void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
            void clear(bool color, bool depth, float r, float g, float b, float a);

        protected:
            void beginFrame() override final;
            void endFrame() override final;

        private:
            struct Texture
            {
                GLuint rawHandle = 0;
                uint32_t width = 0;
                uint32_t height = 0;
            };

            // Counters for assigning unique resource ids.
            // Released ids do not get reused, so this effectively limits the total
            // number of resource allocations during the program lifetime. That's how it is for now.
            uint32_t textureCounter = 0;

            std::unordered_map<uint32_t, Texture> textures;

            std::vector<RenderCommand> renderCommands;

            void bindFrameBuffer(uint32_t handle);
            void bindTexture(GLenum target, uint32_t handle);
            void setTexture(GLenum target, uint32_t handle, uint32_t flags);
        };
    }
}

#endif
