#pragma once

#include "SoloBase.h"
#include "SoloVertexFormat.h"
#include "SoloRenderer.h"


namespace solo
{
    class Effect;

    enum class MeshPrefab
    {
        Quad,
        Cube
    };

    class Mesh final
    {
    public:
        explicit Mesh(Renderer* renderer);
        Mesh(Renderer* renderer, MeshPrefab prefab);
        ~Mesh();

        SL_NONCOPYABLE(Mesh)

        uint32_t addBuffer(const VertexBufferLayout& layout, const float* data, uint32_t elementCount);
        void removeBuffer(uint32_t index);

        uint32_t addPart(const void* indexData, uint32_t indexElementCount);
        void removePart(uint32_t index);
        uint32_t getPartCount() const;

        void draw(Effect* effect);
        void drawPart(Effect* effect, uint32_t part);

        void setPrimitiveType(PrimitiveType type);
        PrimitiveType getPrimitiveType() const;

    protected:
        void initQuadMesh();
        void initCubeMesh();

        void rebuildVertexObject();
        void rebuildEffectBinding(Effect* effect);
        void recalculateMinVertexCount();

        Renderer* renderer;
        Effect* lastEffect = nullptr;

        PrimitiveType primitiveType = PrimitiveType::Triangles;
        std::vector<VertexBufferHandle> vertexBuffers;
        std::vector<IndexBufferHandle> indexBuffers;
        std::vector<uint32_t> vertexCounts;
        uint32_t minVertexCount = 0;
        VertexObjectHandle vertexObjectHandle;
        VertexObjectHandle effectBindingVertexObjectHandle;
    };

    inline void Mesh::setPrimitiveType(PrimitiveType type)
    {
        primitiveType = type;
    }

    inline PrimitiveType Mesh::getPrimitiveType() const
    {
        return primitiveType;
    }

    inline uint32_t Mesh::getPartCount() const
    {
        return static_cast<uint32_t>(indexBuffers.size());
    }
}
