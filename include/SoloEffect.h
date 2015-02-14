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

		size_t getId() const;

		std::string getLog() const;

		bool isValid() const;

	protected:
		Effect(const std::string &vsSrc, const std::string &fsSrc);

		bool valid;
		std::string log;
		size_t id;

		void appendToLog(const std::string &newLog);
	};

	class EffectFactory
	{
		friend class ResourceManager;
		static ptr<Effect> create(const std::string &vsSrc, const std::string &fsSrc);
	};
}
