/*
    Copyright (c) Aleksey Fedotov
    MIT license
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
        OpenGLMesh() {}
        ~OpenGLMesh();

        auto addVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32 override final;
        auto addDynamicVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32 override final;
        void updateDynamicVertexBuffer(u32 index, u32 vertexOffset, const void *data, u32 vertexCount) override final;
        void removeVertexBuffer(u32 index) override final;

        auto addPart(const void *indexData, u32 indexElementCount) -> u32 override final;
        void removePart(u32 index) override final;
        auto getPartCount() const -> u32 override final { return static_cast<u32>(indexBuffers.size()); }

        auto getPrimitiveType() const -> PrimitiveType override final { return primitiveType; }
        void setPrimitiveType(PrimitiveType type) override final { primitiveType = type; }

        void draw(OpenGLEffect *effect);
        void drawPart(u32 part, OpenGLEffect *effect);

    private:
        PrimitiveType primitiveType = PrimitiveType::Triangles;
        vec<GLuint> vertexBuffers;
        vec<VertexBufferLayout> layouts;
        vec<GLuint> indexBuffers;
        vec<u32> indexElementCounts;
        vec<u32> vertexCounts;
        vec<u32> vertexSizes; // TODO use layouts and don't store these
        u32 minVertexCount = 0;
            
        struct VertexArrayCacheEntry
        {
            GLuint handle;
            u32 age;
        };

        umap<OpenGLEffect*, VertexArrayCacheEntry> vertexArrayCache; // TODO clean!

        auto addVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount, bool dynamic) -> u32;

        auto getOrCreateVertexArray(OpenGLEffect *effect) -> GLuint;
        void resetVertexArrayCache();
        void flushVertexArrayCache();
        void updateMinVertexCount();
    };
}

#endif
