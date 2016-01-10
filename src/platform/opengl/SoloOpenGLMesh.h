#pragma once

#include "SoloBase.h"
#include "SoloMesh.h"
#include "SoloVertexFormat.h"
#include <GL/glew.h>

namespace solo
{
    class OpenGLIndexedMeshPart;
    enum class MeshIndexFormat;

    class OpenGLMesh : public Mesh
    {
    public:
        virtual ~OpenGLMesh();

        virtual void resetStorage(unsigned storageId, const float *data, unsigned elementCount, bool dynamic) override;
        virtual void updateStorage(unsigned storageId, const float *data, unsigned elementCount, unsigned updateFromIndex) override;
        
        virtual IndexedMeshPart *addPart(MeshIndexFormat indexFormat) override;
        virtual size_t getPartCount() const override;
        virtual IndexedMeshPart* getPart(unsigned index) const override;

        virtual void draw() override;
        virtual void drawPart(unsigned part) override;

        GLuint getBufferHandle(unsigned storageId) const;

    private:
        friend class Mesh;

        OpenGLMesh(const VertexFormat &vertexFormat);

        static GLenum convertPrimitiveType(MeshPrimitiveType primitiveType);
        static GLenum convertIndexType(MeshIndexFormat indexFormat);

        std::unordered_map<unsigned, GLuint> handles;
        std::unordered_map<unsigned, unsigned> elementCounts;
        std::vector<shared<OpenGLIndexedMeshPart>> parts;
    };

    inline size_t OpenGLMesh::getPartCount() const
    {
        return parts.size();
    }

    inline GLuint OpenGLMesh::getBufferHandle(unsigned storageId) const
    {
        return handles.at(storageId);
    }
}