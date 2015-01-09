#pragma once

#include <GL/glew.h>
#include "SoloEffect.h"

namespace solo
{
	class GLSLEffectVariable;

	class GLSLEffect: public Effect
	{
	public:
		virtual ~GLSLEffect() override;

		static ptr<GLSLEffect> create(const std::string& vsSrc, const std::string& fsSrc);

		virtual void bind() override;

		virtual ptr<EffectVariable> findVariable(const std::string& name) override;

	private:
		GLSLEffect(const std::string& vsSrc, const std::string& fsSrc);

		GLuint _program;
		std::map<std::string, ptr<GLSLEffectVariable>> _variables; // TODO move to the base class (along with some methods)

		GLint createShader(GLuint type, std::string src);
		bool createProgram(GLuint vs, GLuint fs);
		void deleteShader(GLuint vs);
		void discoverVariables();
	};
}
