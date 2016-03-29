#pragma once

#include "SoloBase.h"
#include "SoloVertexFormat.h"
#include "SoloRenderer.h"
#include "SoloVector2.h"
#include "SoloVector3.h"


namespace solo
{
    class Effect;

    enum class MeshPrefab
    {
        Quad,
        Cube
    };

    struct MeshData
    {
        std::vector<Vector3> vertices;
        std::vector<Vector2> uvs;
        std::vector<Vector3> normals;
        std::vector<std::vector<uint16_t>> indices;
    };

    class Mesh final
    {
    public:
        static sptr<Mesh> create();
        static sptr<Mesh> create(MeshPrefab prefab);
        static sptr<Mesh> create(MeshData* data);

        ~Mesh();
        SL_NONCOPYABLE(Mesh)

        uint32_t addVertexBuffer(const VertexBufferLayout& layout, const float* data, uint32_t vertexCount);
        uint32_t addDynamicVertexBuffer(const VertexBufferLayout& layout, const float* data, uint32_t vertexCount);
        void updateDynamicVertexBuffer(uint32_t index, uint32_t vertexOffset, const float* data, uint32_t vertexCount);
        void removeVertexBuffer(uint32_t index);

        uint32_t addPart(const void* indexData, uint32_t indexElementCount);
        void removePart(uint32_t index);
        uint32_t getPartCount() const;

        void draw(Effect* effect);
        void drawPart(Effect* effect, uint32_t part);

        void setPrimitiveType(PrimitiveType type);
        PrimitiveType getPrimitiveType() const;

    private:
        Mesh();
        explicit Mesh(MeshPrefab prefab);
        explicit Mesh(MeshData* data);

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
        std::vector<uint32_t> vertexSizes;
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
