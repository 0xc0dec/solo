#pragma once

#include "SoloBase.h"
#include "SoloMesh.h"
#include "SoloVector3.h"
#include <GL/glew.h>

namespace solo
{
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