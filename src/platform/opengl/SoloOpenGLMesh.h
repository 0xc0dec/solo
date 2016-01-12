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

        virtual void resetStorage(unsigned storageId, const float *data, unsigned elementCount, bool dynamic) override;
        virtual void updateStorage(unsigned storageId, const float *data, unsigned elementCount, unsigned updateFromIndex) override;

        virtual unsigned addIndex(MeshIndexFormat indexFormat) override;
        virtual void removeIndex(unsigned index) override;
        virtual size_t getIndexCount() const override;
        virtual void resetIndexData(unsigned index, const void *data, unsigned elementCount, bool dynamic) override;
        virtual void updateIndexData(unsigned index, const void *data, unsigned elementCount, unsigned updateFromIndex) override;

        virtual MeshPrimitiveType getIndexPrimitiveType(unsigned index) override;
        virtual void setIndexPrimitiveType(unsigned index, MeshPrimitiveType primitiveType) override;

        virtual void draw() override;
        virtual void drawIndex(unsigned index) override;

        GLuint getBufferHandle(unsigned storageId) const;

    private:
        friend class Mesh;

        OpenGLMesh(const VertexFormat &vertexFormat);

        std::vector<GLuint> indexHandles;
        std::vector<MeshIndexFormat> indexFormats;
        std::vector<MeshPrimitiveType> indexPrimitiveTypes;
        std::vector<unsigned> indexElementCounts;
        std::vector<GLuint> handles;
        std::vector<GLuint> elementCounts;
    };

    inline size_t OpenGLMesh::getIndexCount() const
    {
        return indexHandles.size();
    }

    inline MeshPrimitiveType OpenGLMesh::getIndexPrimitiveType(unsigned index)
    {
        return indexPrimitiveTypes[index];
    }

    inline void OpenGLMesh::setIndexPrimitiveType(unsigned index, MeshPrimitiveType primitiveType)
    {
        indexPrimitiveTypes[index] = primitiveType;
    }

    inline GLuint OpenGLMesh::getBufferHandle(unsigned storageId) const
    {
        return handles.at(storageId);
    }
}