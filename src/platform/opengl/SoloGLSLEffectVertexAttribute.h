#pragma once

#include "SoloBase.h"
#include "SoloEffectVertexAttribute.h"
#include <GL/glew.h>

namespace solo
{
    class GLSLEffectVertexAttribute: public EffectVertexAttribute
    {
    public:
        GLuint getLocation() const;

    private:
        friend class GLSLEffect;

        GLSLEffectVertexAttribute(const std::string& name, GLuint location);

        GLuint location = 0;
    };

    inline GLuint GLSLEffectVertexAttribute::getLocation() const
    {
        return location;
    }
}
