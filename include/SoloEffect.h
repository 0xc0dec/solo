#pragma once

#include "SoloBase.h"

namespace solo
{
	class EffectVariable;

	class Effect
	{
	public:
		virtual ~Effect() {}

		virtual void bind() = 0;

		EffectVariable* findVariable(const std::string& name) const;

	protected:
		Effect() {}

		std::unordered_map<std::string, shared<EffectVariable>> variables;

	private:
		Effect(const Effect& other) = delete;
		Effect(Effect&& other) = delete;
		Effect& operator=(const Effect& other) = delete;
		Effect& operator=(Effect&& other) = delete;
	};

	class EffectFactory
	{
		friend class ResourceManager;
		static shared<Effect> create(const std::string &vsSrc, const std::string &fsSrc);
	};
}
