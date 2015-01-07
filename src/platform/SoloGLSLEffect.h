#pragma once

#include <GL/glew.h>
#include "SoloEffect.h"

namespace solo
{
	class GLSLEffectVariable;

	class GLSLEffect: public Effect
	{
	public:
		GLSLEffect(std::string const& vsSrc, std::string const& fsSrc);
		virtual ~GLSLEffect() override;

		virtual void bind() override;

		virtual ptr<EffectVariable> findVariable(const std::string& name) override;

	private:
		GLuint _program;
		std::map<std::string, ptr<GLSLEffectVariable>> _variables; // TODO move to the base class (along with some methods)

		GLint createShader(GLuint type, std::string src);
		bool createProgram(GLuint vs, GLuint fs);
		void deleteShader(GLuint vs);
		void discoverVariables();
	};
}
