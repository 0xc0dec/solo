#pragma once

#include "SoloMesh2.h"
#include "SoloVertexFormat.h"
#include <GL/glew.h>

namespace solo
{
    class OpenGLMesh2 : public Mesh2
    {
    public:
	    virtual void setVertexData(float *data, unsigned vertexCount, unsigned firstVertexIndex) override;

    private:
		friend class Mesh2;

		OpenGLMesh2(const VertexFormat &vertexFormat, bool dynamic);

		GLuint vertexBufferHandle = 0;
		unsigned lastVertexCount = 0;
    };
}
