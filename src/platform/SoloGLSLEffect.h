#pragma once

#include <GL/glew.h>
#include "SoloEffect.h"

namespace solo
{
	class GLSLEffectVariable;

	class GLSLEffect: public Effect
	{
	public:
		virtual ~GLSLEffect();

		virtual void bind() override;

		virtual EffectVariable* findVariable(const std::string& name) override;

	private:
		friend class EffectFactory;

		GLSLEffect(const std::string& vsSrc, const std::string& fsSrc);
		GLSLEffect(const GLSLEffect& other) = delete;
		GLSLEffect(GLSLEffect&& other) = delete;
		GLSLEffect& operator=(const GLSLEffect& other) = delete;
		GLSLEffect& operator=(GLSLEffect&& other) = delete;

		GLuint program = 0;
		std::map<std::string, shared<GLSLEffectVariable>> variables; // TODO move to the base class (along with some methods)

		GLint createShader(GLuint type, std::string src);
		bool createProgram(GLuint vs, GLuint fs);
		void deleteShader(GLuint vs);
		void discoverVariables();
	};
}
