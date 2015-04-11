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

		std::string getLog() const;
		bool isValid() const;

	protected:
		Effect() {}

		void appendToLog(const std::string &newLog);

		std::map<std::string, shared<EffectVariable>> variables; // TODO move to the base class (along with some methods)

		bool valid = false;
		std::string log;

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
