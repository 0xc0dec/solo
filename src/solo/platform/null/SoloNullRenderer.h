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

#include "SoloRenderer.h"

namespace solo
{
    class NullRenderer final : public Renderer
    {
    public:
        void beginFrame() override final {}
        void endFrame() override final {}

        auto createTexture() -> TextureHandle override final { return EmptyTextureHandle; }
        void destroyTexture(const TextureHandle& handle) override final {}
        void set2DTexture(const TextureHandle& handle) override final {}
        void set2DTexture(const TextureHandle& handle, uint32_t flags) override final {}
        void set2DTexture(const TextureHandle& handle, uint32_t flags, float anisotropyLevel) override final {}
        void setCubeTexture(const TextureHandle& handle) override final {}
        void setCubeTexture(const TextureHandle& handle, uint32_t flags) override final {}
        void setCubeTexture(const TextureHandle& handle, uint32_t flags, float anisotropyLevel) override final {}
        void update2DTexture(const TextureHandle& handle, TextureFormat format, uint32_t width, uint32_t height, const void* data) override final {}
        void updateCubeTexture(const TextureHandle& handle, CubeTextureFace face, TextureFormat format, uint32_t width,
            uint32_t height, const void* data) override final {}
        void generateRectTextureMipmaps(const TextureHandle& handle) override final {}
        void generateCubeTextureMipmaps(const TextureHandle& handle) override final {}

        auto createFrameBuffer() -> FrameBufferHandle override final { return EmptyFrameBufferHandle; }
        void destroyFrameBuffer(const FrameBufferHandle& handle) override final {}
        void setFrameBuffer(const FrameBufferHandle& handle) override final {}
        void updateFrameBuffer(const FrameBufferHandle& handle, const std::vector<TextureHandle>& attachmentHandles) override final {}

        auto createVertexBuffer(const VertexBufferLayout& layout, const void* data, uint32_t vertexCount)
            -> VertexBufferHandle override final { return EmptyVertexBufferHandle; }
        auto createDynamicVertexBuffer(const VertexBufferLayout& layout, const void* data, uint32_t vertexCount)
            -> VertexBufferHandle override final { return EmptyVertexBufferHandle; }
        void updateDynamicVertexBuffer(const VertexBufferHandle& handle, const void* data, uint32_t offset, uint32_t vertexCount) override final {}
        void destroyVertexBuffer(const VertexBufferHandle& handle) override final {}

        auto createIndexBuffer(const void* data, uint32_t elementSize, uint32_t elementCount)
            -> IndexBufferHandle override final { return EmptyIndexBufferHandle; }
        void destroyIndexBuffer(const IndexBufferHandle& handle) override final {}

        auto createProgram(const char* vsSrc, const char* fsSrc) -> ProgramHandle override final { return EmptyProgramHandle; }
        void destroyProgram(const ProgramHandle& handle) override final {}
        void setProgram(const ProgramHandle& handle) override final {}

        auto createVertexProgramBinding(const VertexBufferHandle* bufferHandles, uint32_t bufferCount, ProgramHandle programHandle)
            -> VertexProgramBindingHandle override final { return EmptyVertexProgramBindingHandle; }
        void destroyVertexProgramBinding(const VertexProgramBindingHandle& handle) override final {}

        auto createUniform(const char* name, UniformType type, ProgramHandle programHandle)
            -> UniformHandle override final { return EmptyUniformHandle; }
        void destroyUniform(const UniformHandle& handle) override final {}
        void setUniform(const UniformHandle& handle, const void* value, uint32_t count) override final {}

        void drawIndexed(PrimitiveType primitiveType, const VertexProgramBindingHandle& bindingHandle,
            const IndexBufferHandle& indexBufferHandle) override final {}
        void draw(PrimitiveType primitiveType, const VertexProgramBindingHandle& bindingHandle, uint32_t vertexCount) override final {}
    };
}