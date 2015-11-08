#pragma once

#include "SoloBase.h"

namespace solo
{
	class EffectVariable;
	enum class DeviceMode;

	class Effect
	{
	public:
		static shared<Effect> create(DeviceMode mode, const std::string &vsSrc, const std::string &fsSrc);

		Effect(const Effect& other) = delete;
		Effect(Effect&& other) = delete;
		Effect& operator=(const Effect& other) = delete;
		Effect& operator=(Effect&& other) = delete;
		virtual ~Effect() {}

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual EffectVariable* findVariable(const std::string& name) const;

	protected:
		Effect() {}

		std::unordered_map<std::string, shared<EffectVariable>> variables;
	};
}
