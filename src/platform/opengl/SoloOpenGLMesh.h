#pragma once

#include "SoloBase.h"
#include "SoloMesh.h"
#include "SoloVector3.h"
#include "SoloVertexFormat.h"
#include <GL/glew.h>

namespace solo
{
    class OpenGLIndexedMeshPart: public IndexedMeshPart
    {
    public:
        virtual ~OpenGLIndexedMeshPart();

        virtual void resetIndexData(MeshIndexFormat indexFormat, float* data, unsigned elementCount, bool dynamic) override;
        virtual void updateIndexData(float *data, unsigned elementCount, unsigned updateFromIndex) override;

        virtual unsigned getElementCount() const override;
        virtual MeshIndexFormat getIndexFormat() const override;

        GLuint getBufferHandle() const;

    private:
        friend class OpenGLMesh2;

        OpenGLIndexedMeshPart();

        static unsigned getElementSize(MeshIndexFormat indexFormat);
        static GLenum convertPrimitiveType(PrimitiveType primitiveType);

        GLuint bufferHandle = 0;
        MeshIndexFormat indexFormat = MeshIndexFormat::UnsignedShort;
        unsigned bufferElementCount = 0;
        unsigned elementSize = 0;
    };

    inline unsigned OpenGLIndexedMeshPart::getElementCount() const
    {
        return bufferElementCount;
    }

    inline MeshIndexFormat OpenGLIndexedMeshPart::getIndexFormat() const
    {
        return indexFormat;
    }

    inline GLuint OpenGLIndexedMeshPart::getBufferHandle() const
    {
        return bufferHandle;
    }


    class OpenGLMesh2 : public Mesh2
    {
    public:
        virtual ~OpenGLMesh2();

        virtual void resetVertexData(const VertexFormat &format, float *data, unsigned elementCount, bool dynamic) override;
        virtual void updateVertexData(float *data, unsigned elementCount, unsigned updateFromIndex) override;
        
        virtual IndexedMeshPart *addIndexedPart() override;
        virtual size_t getPartCount() const override;

        virtual void draw() override;
        virtual void drawIndexedPart(unsigned part) override;

    private:
        friend class Mesh2;

        OpenGLMesh2();

        static GLenum convertPrimitiveType(PrimitiveType primitiveType);
        static GLenum convertIndexType(MeshIndexFormat indexFormat);

        GLuint bufferHandle = 0;
        unsigned bufferElementCount = 0;
        std::vector<shared<OpenGLIndexedMeshPart>> parts;
    };

    inline size_t OpenGLMesh2::getPartCount() const
    {
        return parts.size();
    }


    class OpenGLMesh : public Mesh
    {
    public:
        virtual ~OpenGLMesh();

        virtual void draw() override;

        virtual void setVertices(const std::vector<Vector3> &vertices) override;
        virtual void setNormals(const std::vector<Vector3> &normals) override;
        virtual void setUVs(const std::vector<Vector2> &uvs) override;
        virtual void setIndices(const std::vector<uint16_t> &indices) override;

    private:
        friend class Mesh;

        OpenGLMesh();

        GLuint vertexArrayHandle = 0;
        GLuint vertexBufferHandle = 0;
        GLuint normalBufferHandle = 0;
        GLuint uvBufferHandle = 0;
        GLuint indicesBufferHandle = 0;

        size_t verticesCount = 0;
        size_t normalsCount = 0;
        size_t uvsCount = 0;
        size_t indicesCount = 0;
    };
}