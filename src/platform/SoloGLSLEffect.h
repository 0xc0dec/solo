#pragma once

#include <GL/glew.h>
#include "SoloEffect.h"

namespace solo
{
	class GLSLEffect: public Effect
	{
	public:
		GLSLEffect(const std::string& vsSrc, const std::string& fsSrc);
		virtual ~GLSLEffect();

		virtual void bind() override;

	private:
		GLSLEffect(const GLSLEffect& other) = delete;
		GLSLEffect(GLSLEffect&& other) = delete;
		GLSLEffect& operator=(const GLSLEffect& other) = delete;
		GLSLEffect& operator=(GLSLEffect&& other) = delete;

		GLuint program { 0 };

		void deleteShader(GLuint vs);
		void discoverVariables();
	};
}
