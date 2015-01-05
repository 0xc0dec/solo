#pragma once

#include <GL/glew.h>
#include "../SoloEffect.h"

namespace solo
{
	class GLSLEffect: public Effect
	{
	public:
		static ptr<GLSLEffect> create(std::string const& vsSrc, std::string const& fsSrc);

	private:
		explicit GLSLEffect(std::string const& vsSrc, std::string const& fsSrc);
		~GLSLEffect();

		GLuint _program;

		GLint tryCreateShader(GLuint type, std::string src);
		GLint tryCreateProgram(GLuint vs, GLuint fs);
	};
}
