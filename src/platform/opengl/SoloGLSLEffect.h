#pragma once

#include "SoloEffect.h"
#include <GL/glew.h>

namespace solo
{
	class GLSLEffect: public Effect
	{
	public:
		virtual ~GLSLEffect();

		virtual void bind() override;

	private:
		friend class EffectFactory;

		GLSLEffect(const std::string& vsSrc, const std::string& fsSrc);

		void deleteShader(GLuint vs);
		void discoverVariables();

		GLuint program { 0 };
	};
}
