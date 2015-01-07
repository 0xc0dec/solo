#pragma once

#include <GL/glew.h>
#include "SoloEffect.h"

namespace solo
{
	class GLSLEffect: public Effect
	{
	public:
		GLSLEffect(std::string const& vsSrc, std::string const& fsSrc);
		virtual ~GLSLEffect() override;

		virtual void bind() override;

	private:
		GLuint _program;

		GLint tryCreateShader(GLuint type, std::string src);
		GLint tryCreateProgram(GLuint vs, GLuint fs);
	};
}
