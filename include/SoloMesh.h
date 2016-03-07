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
        static shared<Mesh> createPrefab(Renderer* renderer, MeshPrefab prefab);

        Mesh(Renderer* renderer);
        ~Mesh();

        SL_NONCOPYABLE(Mesh)

        int addBuffer(const VertexBufferLayout& layout, const float* data, int elementCount);
        void removeBuffer(int index);

        int addIndex(const void* data, int elementCount);
        void removeIndex(int index);
        int getIndexCount() const;

        void getEffectBinding() {} // TODO

        void draw();
        void drawIndex(int index);

        void setPrimitiveType(PrimitiveType type);
        PrimitiveType getPrimitiveType() const;

    protected:
        static shared<Mesh> createQuadMesh(Renderer* renderer);
        static shared<Mesh> createBoxMesh(Renderer* renderer);
        void rebuildVertexObject();

        Renderer* renderer;

        PrimitiveType primitiveType = PrimitiveType::Triangles;
        std::vector<VertexBufferHandle> vertexBuffers;
        std::vector<IndexBufferHandle> indexBuffers;
        std::unordered_map<int, VertexObjectHandle> perEffectVertexObjectHandles;
        VertexObjectHandle vertexObjectHandle;
    };

    inline void Mesh::setPrimitiveType(PrimitiveType type)
    {
        primitiveType = type;
    }

    inline PrimitiveType Mesh::getPrimitiveType() const
    {
        return primitiveType;
    }

    inline int Mesh::getIndexCount() const
    {
        return indexBuffers.size();
    }
}
