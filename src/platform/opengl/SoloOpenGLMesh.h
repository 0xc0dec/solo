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

        virtual void resetVertexData(const VertexFormat &format, const float *data, unsigned elementCount, bool dynamic) override;
        virtual void updateVertexData(const float *data, unsigned elementCount, unsigned updateFromIndex) override;
        
        virtual IndexedMeshPart *addPart() override;
        virtual size_t getPartCount() const override;
        virtual IndexedMeshPart* getPart(unsigned index) const override;

        virtual void draw() override;
        virtual void drawIndexedPart(unsigned part) override;

        GLuint getBufferHandle() const;

    private:
        friend class Mesh;

        OpenGLMesh();

        static GLenum convertPrimitiveType(MeshPrimitiveType primitiveType);
        static GLenum convertIndexType(MeshIndexFormat indexFormat);

        GLuint bufferHandle = 0;
        unsigned bufferElementCount = 0;
        std::vector<shared<OpenGLIndexedMeshPart>> parts;
    };

    inline size_t OpenGLMesh::getPartCount() const
    {
        return parts.size();
    }

    inline GLuint OpenGLMesh::getBufferHandle() const
    {
        return bufferHandle;
    }
}