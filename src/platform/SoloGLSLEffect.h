#pragma once

#include <GL/glew.h>
#include "../SoloEffect.h"

namespace solo
{
	class GLSLEffect: public Effect
	{
	public:
		explicit GLSLEffect(std::string const& vsSrc, std::string const& fsSrc);
		~GLSLEffect();

	private:
		GLuint _program;

		GLint tryCreateShader(GLuint type, std::string src);
		GLint tryCreateProgram(GLuint vs, GLuint fs);
	};
}
