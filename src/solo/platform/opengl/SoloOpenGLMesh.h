#pragma once

#include "SoloMesh.h"
#include "SoloOpenGLRenderer.h"


namespace solo
{
    class OpenGLEffect;

    class OpenGLMesh final: public Mesh
    {
    public:
        explicit OpenGLMesh(Device* device);
        OpenGLMesh(Device* device, MeshData* data);
        OpenGLMesh(Device* device, MeshPrefab prefab);
        ~OpenGLMesh();

        auto addVertexBuffer(const VertexBufferLayout& layout, const float* data, uint32_t vertexCount) -> uint32_t override final;
        auto addDynamicVertexBuffer(const VertexBufferLayout& layout, const float* data, uint32_t vertexCount) -> uint32_t override final;
        void updateDynamicVertexBuffer(uint32_t index, uint32_t vertexOffset, const float* data, uint32_t vertexCount) override final;
        void removeVertexBuffer(uint32_t index) override final;

        auto addPart(const void* indexData, uint32_t indexElementCount) -> uint32_t override final;
        void removePart(uint32_t index) override final;
        auto getPartCount() const -> uint32_t override final;

        void draw(Effect* effect) override final;
        void drawPart(Effect* effect, uint32_t part) override final;

        auto getPrimitiveType() const -> PrimitiveType override final;
        void setPrimitiveType(PrimitiveType type) override final;

    private:
        auto addVertexBuffer(VertexBufferHandle bufferHandle, const VertexBufferLayout& layout, uint32_t vertexCount) -> uint32_t;

        void rebuildEffectBinding(Effect* effect);
        void recalculateMinVertexCount();

        OpenGLRenderer* renderer = nullptr;
        OpenGLEffect* lastEffect = nullptr;

        PrimitiveType primitiveType = PrimitiveType::Triangles;
        std::vector<VertexBufferHandle> vertexBuffers;
        std::vector<IndexBufferHandle> indexBuffers;
        std::vector<uint32_t> vertexCounts;
        std::vector<uint32_t> vertexSizes;
        uint32_t minVertexCount = 0;
        VertexProgramBindingHandle programBinding = EmptyVertexProgramBindingHandle;
    };

    inline void OpenGLMesh::setPrimitiveType(PrimitiveType type)
    {
        primitiveType = type;
    }

    inline auto OpenGLMesh::getPrimitiveType() const -> PrimitiveType
    {
        return primitiveType;
    }

    inline auto OpenGLMesh::getPartCount() const -> uint32_t
    {
        return static_cast<uint32_t>(indexBuffers.size());
    }
}