#pragma once

#include "SoloBase.h"

namespace solo
{
	class EffectVariable;

	class Effect
	{
	public:
		virtual ~Effect() {}

		virtual EffectVariable* findVariable(const std::string &name) = 0;
		virtual void bind() = 0;

		std::string getLog() const;
		bool isValid() const;

	protected:
		Effect();

		bool valid;
		std::string log;

		void appendToLog(const std::string &newLog);
	};

	class EffectFactory
	{
		friend class ResourceManager;
		static shared<Effect> create(const std::string &vsSrc, const std::string &fsSrc);
	};
}
