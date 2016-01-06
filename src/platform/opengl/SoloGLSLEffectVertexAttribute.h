#pragma once

#include "SoloBase.h"
#include "SoloEffectVertexAttribute.h"
#include <GL/glew.h>

namespace solo
{
    class GLSLEffectVertexAttribute: public EffectVertexAttribute
    {
    public:
		GLint getLocation() const;

    private:
        friend class GLSLEffect;

		explicit GLSLEffectVertexAttribute(const std::string& name, GLint location);

		GLint location = 0;
    };

	inline GLint GLSLEffectVertexAttribute::getLocation() const
	{
		return location;
	}
}
