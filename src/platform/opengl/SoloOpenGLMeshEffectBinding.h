#pragma once

#include "../../SoloMeshEffectBinding.h"
#include <GL/glew.h>

namespace solo
{
    class OpenGLMeshEffectBinding: public MeshEffectBinding
    {
    public:
        virtual ~OpenGLMeshEffectBinding();

        virtual void bind() override;
        virtual void unbind() override;

    private:
        friend class MeshEffectBinding;

        OpenGLMeshEffectBinding(Mesh2 *mesh, Effect *effect);

        GLuint vertexArrayHandle = 0;
    };
}
