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
		Effect() {}

		void appendToLog(const std::string &newLog);

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
