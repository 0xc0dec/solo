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

    namespace gl
    {
        class Effect;

        class Mesh final : public solo::Mesh
        {
        public:
            Mesh() {}
            ~Mesh();

            auto addVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32 override final;
            auto addDynamicVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32 override final;
            void updateDynamicVertexBuffer(u32 index, u32 vertexOffset, const void *data, u32 vertexCount) override final;
            void removeVertexBuffer(u32 index) override final;

            auto addPart(const void *indexData, u32 indexElementCount) -> u32 override final;
            void removePart(u32 index) override final;
            auto getPartCount() const -> u32 override final;

            auto getPrimitiveType() const -> PrimitiveType override final;
            void setPrimitiveType(PrimitiveType type) override final;

            void draw(gl::Effect *effect);
            void drawPart(u32 part, gl::Effect *effect);

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

            umap<gl::Effect*, VertexArrayCacheEntry> vertexArrayCache; // TODO clean!

            auto addVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount, bool dynamic) -> u32;

            auto getOrCreateVertexArray(gl::Effect *effect) -> GLuint;
            void resetVertexArrayCache();
            void flushVertexArrayCache();
            void updateMinVertexCount();
        };

        inline void Mesh::setPrimitiveType(PrimitiveType type)
        {
            primitiveType = type;
        }

        inline auto Mesh::getPrimitiveType() const -> PrimitiveType
        {
            return primitiveType;
        }

        inline auto Mesh::getPartCount() const -> u32
        {
            return static_cast<u32>(indexBuffers.size());
        }
    }
}

#endif
