#pragma once

#include <GL/glew.h>
#include "SoloBase.h"
#include "SoloEffectVariable.h"

namespace solo
{
	class GLSLEffectVariable : public EffectVariable
	{
	public:
		GLSLEffectVariable(const std::string& name, GLint location, GLenum type, unsigned index = 0);

	private:
		GLint _location;
		GLenum _type;
		unsigned _index;
	};
}