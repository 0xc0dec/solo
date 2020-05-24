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

        auto addVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32 override;
        auto addDynamicVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32 override;
        void updateDynamicVertexBuffer(u32 index, u32 vertexOffset, const void *data, u32 vertexCount) override;
        void removeVertexBuffer(u32 index) override;

        auto addPart(const void *indexData, u32 indexElementCount, IndexElementSize elementSize) -> u32 override;
        void removePart(u32 part) override;
        auto partCount() const -> u32 override { return static_cast<u32>(indexBuffers_.size()); }
    	auto partElementSize(u32 part) -> IndexElementSize override { return indexElementSizes_.at(part); }

        auto primitiveType() const -> PrimitiveType override { return primitiveType_; }
        void setPrimitiveType(PrimitiveType type) override { primitiveType_ = type; }

        void render(OpenGLEffect *effect);
        void renderPart(u32 part, OpenGLEffect *effect);

    private:
        PrimitiveType primitiveType_ = PrimitiveType::Triangles;
        vec<GLuint> vertexBuffers_;
        vec<VertexBufferLayout> layouts_;
        vec<GLuint> indexBuffers_;
        vec<u32> indexElementCounts_;
    	vec<IndexElementSize> indexElementSizes_;
        vec<u32> vertexCounts_;
        u32 minVertexCount_ = 0;
            
        struct VertexArrayCacheEntry
        {
            GLuint handle;
            u32 age;
        };

        umap<OpenGLEffect*, VertexArrayCacheEntry> vertexArrayCache_;

        auto addVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount, bool dynamic) -> u32;

        auto getOrCreateVertexArray(OpenGLEffect *effect) -> GLuint;
        void resetVertexArrayCache();
        void flushVertexArrayCache();
        void updateMinVertexCount();
    };
}

#endif
