#pragma once

#include "SoloBase.h"

namespace solo
{
	class MaterialParameter;
	class Effect;

	class RenderState
	{
	public:
		RenderState() {}
		virtual ~RenderState() {}

		MaterialParameter* getParameter(const std::string &name);

	protected:
		std::map<std::string, shared<MaterialParameter>> parameters;
	};
}