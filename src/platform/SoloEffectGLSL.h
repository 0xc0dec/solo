#ifndef __SOLO_EFFECT_GLSL_H__
#define __SOLO_EFFECT_GLSL_H__

#include <GL/glew.h>
#include "../SoloEffect.h"

namespace solo
{
	class EffectGLSL: public Effect
	{
	public:
		explicit EffectGLSL(std::string const& vsShaderSrc, std::string const& fsShaderSrc);
		~EffectGLSL();

	private:
		GLuint _program;

		GLint _tryCreateShader(GLuint type, std::string src);
		GLint _tryCreateProgram(GLuint vs, GLuint fs);
	};
}

#endif