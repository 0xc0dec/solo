#pragma once

#include "SoloCommon.h"
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
        static auto create() -> sptr<Mesh>;
        static auto create(MeshPrefab prefab) -> sptr<Mesh>;
        static auto create(MeshData* data) -> sptr<Mesh>;

        ~Mesh();
        SL_NONCOPYABLE(Mesh)

        auto addVertexBuffer(const VertexBufferLayout& layout, const float* data, uint32_t vertexCount) -> uint32_t;
        auto addDynamicVertexBuffer(const VertexBufferLayout& layout, const float* data, uint32_t vertexCount) -> uint32_t;
        void updateDynamicVertexBuffer(uint32_t index, uint32_t vertexOffset, const float* data, uint32_t vertexCount);
        void removeVertexBuffer(uint32_t index);

        auto addPart(const void* indexData, uint32_t indexElementCount) -> uint32_t;
        void removePart(uint32_t index);
        auto getPartCount() const -> uint32_t;

        void draw(Effect* effect);
        void drawPart(Effect* effect, uint32_t part);

        void setPrimitiveType(PrimitiveType type);
        auto getPrimitiveType() const -> PrimitiveType;

    private:
        Mesh();
        explicit Mesh(MeshPrefab prefab);
        explicit Mesh(MeshData* data);

        void initQuadMesh();
        void initCubeMesh();

        auto addVertexBuffer(VertexBufferHandle bufferHandle, const VertexBufferLayout& layout, uint32_t vertexCount) -> uint32_t;

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

    inline auto Mesh::getPrimitiveType() const -> PrimitiveType
    {
        return primitiveType;
    }

    inline auto Mesh::getPartCount() const -> uint32_t
    {
        return static_cast<uint32_t>(indexBuffers.size());
    }
}
