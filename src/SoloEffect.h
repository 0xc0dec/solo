#pragma once

#include "SoloIEffect.h"

namespace solo
{
	class Effect: public IEffect
	{
	public:
		static ptr<Effect> create(const std::string &vsSrc, const std::string &fsSrc);

		virtual size_t getId() const override;

		virtual std::string getLog() const override;

		virtual bool isValid() const override;

	protected:
		Effect(const std::string &vsSrc, const std::string &fsSrc);
		virtual ~Effect();

		bool _valid;
		std::string _log;
		size_t _id;

		void appendToLog(const std::string &newLog);
	};
}
