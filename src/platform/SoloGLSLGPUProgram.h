#ifndef __SOLO_GLSL_GPU_PROGRAM_H__
#define __SOLO_GLSL_GPU_PROGRAM_H__

#include <GL/glew.h>
#include "../SoloGPUProgram.h"

namespace solo
{
	class GLSLGPUProgram: public GPUProgram
	{
	public:
		GLSLGPUProgram(str const& vsShaderSrc, str const& fsShaderSrc);
		~GLSLGPUProgram();

	private:
		GLuint _program;

		GLint _tryCreateShader(GLuint type, str src);
		GLint _tryCreateProgram(GLuint vs, GLuint fs);
	};
}

#endif