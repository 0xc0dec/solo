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

            auto addVertexBuffer(const VertexBufferLayout &layout, const void *data, uint32_t vertexCount) -> uint32_t override final;
            auto addDynamicVertexBuffer(const VertexBufferLayout &layout, const void *data, uint32_t vertexCount) -> uint32_t override final;
            void updateDynamicVertexBuffer(uint32_t index, uint32_t vertexOffset, const void *data, uint32_t vertexCount) override final;
            void removeVertexBuffer(uint32_t index) override final;

            auto addPart(const void *indexData, uint32_t indexElementCount) -> uint32_t override final;
            void removePart(uint32_t index) override final;
            auto getPartCount() const -> uint32_t override final;

            auto getPrimitiveType() const -> PrimitiveType override final;
            void setPrimitiveType(PrimitiveType type) override final;

            void draw() const;
            void drawPart(uint32_t part) const;

        private:
            PrimitiveType primitiveType = PrimitiveType::Triangles;
            std::vector<GLuint> vertexBuffers;
            std::vector<VertexBufferLayout> layouts;
            std::vector<GLuint> indexBuffers;
            std::vector<uint32_t> indexElementCounts;
            std::vector<uint32_t> vertexCounts;
            std::vector<uint32_t> vertexSizes; // TODO use layouts and don't store these
            uint32_t minVertexCount = 0;
            // TODO avoid the need for mutables
            mutable GLuint vertexArray = 0;
            mutable bool dirtyVertexArray = true;

            auto addVertexBuffer(const VertexBufferLayout &layout, const void *data, uint32_t vertexCount, bool dynamic) -> uint32_t;

            void rebuildVertexArray() const;
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

        inline auto Mesh::getPartCount() const -> uint32_t
        {
            return static_cast<uint32_t>(indexBuffers.size());
        }
    }
}

#endif
