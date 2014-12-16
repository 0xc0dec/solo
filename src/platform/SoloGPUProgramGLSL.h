#ifndef __SOLO_GPU_PROGRAM_GLSL_H__
#define __SOLO_GPU_PROGRAM_GLSL_H__

#include <GL/glew.h>
#include "../SoloGPUProgram.h"

namespace solo
{
	class GPUProgramGLSL: public GPUProgram
	{
	public:
		explicit GPUProgramGLSL(std::string const& vsShaderSrc, std::string const& fsShaderSrc);
		~GPUProgramGLSL();

	private:
		GLuint _program;

		GLint _tryCreateShader(GLuint type, std::string src);
		GLint _tryCreateProgram(GLuint vs, GLuint fs);
	};
}

#endif