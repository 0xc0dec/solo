#ifndef __SOLO_GLSL_EFFECT_H__
#define __SOLO_GLSL_EFFECT_H__

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

#endif