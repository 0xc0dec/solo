#pragma once

#include <GL/glew.h>
#include "SoloEffect.h"

namespace solo
{
	class GLSLEffect: public Effect
	{
	public:
		virtual ~GLSLEffect() override;

		static ptr<GLSLEffect> create(std::string const& vsSrc, std::string const& fsSrc);

	private:
		GLSLEffect(std::string const& vsSrc, std::string const& fsSrc);

		GLuint _program;

		GLint tryCreateShader(GLuint type, std::string src);
		GLint tryCreateProgram(GLuint vs, GLuint fs);
	};
}
