#pragma once

#include "SoloBase.h"

namespace solo
{
	class EffectVariable;
	enum class EngineMode;

	class Effect
	{
	public:
		static shared<Effect> create(EngineMode engineMode, const std::string &vsSrc, const std::string &fsSrc);

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
}
