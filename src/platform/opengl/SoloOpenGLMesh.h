#pragma once

#include "SoloBase.h"
#include "SoloMesh.h"
#include "SoloVertexFormat.h"
#include <GL/glew.h>

namespace solo
{
    enum class MeshIndexFormat;

    class OpenGLMesh : public Mesh
    {
    public:
        virtual ~OpenGLMesh();

        virtual void resetStorage(int storageId, const float* data, int elementCount, bool dynamic) override;
        virtual void updateStorage(int storageId, const float* data, int elementCount, int updateFromIndex) override;

        virtual int addIndex(MeshIndexFormat indexFormat) override;
        virtual void removeIndex(int index) override;
        virtual size_t getIndexCount() const override;
        virtual void resetIndexData(int index, const void* data, int elementCount, bool dynamic) override;
        virtual void updateIndexData(int index, const void* data, int elementCount, int updateFromIndex) override;

        virtual MeshPrimitiveType getIndexPrimitiveType(int index) override;
        virtual void setIndexPrimitiveType(int index, MeshPrimitiveType primitiveType) override;

        virtual void draw() override;
        virtual void drawIndex(int index) override;

        GLuint getBufferHandle(int storageId) const;

    private:
        friend class Mesh;

        OpenGLMesh(const VertexFormat& vertexFormat);

        std::vector<GLuint> indexHandles;
        std::vector<MeshIndexFormat> indexFormats;
        std::vector<MeshPrimitiveType> indexPrimitiveTypes;
        std::vector<int> indexElementCounts;
        std::vector<GLuint> handles;
        std::vector<GLuint> elementCounts;
    };

    inline size_t OpenGLMesh::getIndexCount() const
    {
        return indexHandles.size();
    }

    inline MeshPrimitiveType OpenGLMesh::getIndexPrimitiveType(int index)
    {
        return indexPrimitiveTypes[index];
    }

    inline void OpenGLMesh::setIndexPrimitiveType(int index, MeshPrimitiveType primitiveType)
    {
        indexPrimitiveTypes[index] = primitiveType;
    }

    inline GLuint OpenGLMesh::getBufferHandle(int storageId) const
    {
        return handles.at(storageId);
    }
}