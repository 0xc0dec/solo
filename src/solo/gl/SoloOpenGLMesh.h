/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloMesh.h"
#include "SoloOpenGL.h"

namespace solo
{
    class Effect;
    class OpenGLEffect;

    class OpenGLMesh final : public Mesh
    {
    public:
        OpenGLMesh() = default;
        ~OpenGLMesh();

        auto addVertexBuffer(const VertexBufferLayout &layout, const vec<float> &data, u32 vertexCount) -> u32 override;
        auto addDynamicVertexBuffer(const VertexBufferLayout &layout, const vec<float> &data, u32 vertexCount) -> u32 override;
        void updateVertexBuffer(u32 index, u32 vertexOffset, const void *data, u32 vertexCount) override;
        void removeVertexBuffer(u32 index) override;

        auto addIndexBuffer(const vec<u32> &data, u32 indexElementCount) -> u32 override;
        void removeIndexBuffer(u32 index) override;

        void render(OpenGLEffect *effect);
        void renderIndex(u32 index, OpenGLEffect *effect);

    private:
        vec<GLuint> vertexBuffers_;
        vec<GLuint> indexBuffers_;

        struct VertexArrayCacheEntry
        {
            GLuint handle;
            u32 age;
        };

        umap<OpenGLEffect *, VertexArrayCacheEntry> vertexArrayCache_;

        void addVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount, bool dynamic);

        auto getOrCreateVertexArray(OpenGLEffect *effect) -> GLuint;
        void clearVertexArrayCache();
        void flushVertexArrayCache();
    };
}

#endif
