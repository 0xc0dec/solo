#pragma once

#include "SoloIEffect.h"

namespace solo
{
	class Effect: public IEffect
	{
	public:
		Effect(const std::string &vsSrc, const std::string &fsSrc);
		virtual ~Effect();

		virtual bool isValid() const override;

		virtual std::string getLog() const override;

		virtual size_t getId() const override;

	protected:
		bool _valid;
		std::string _log;
		size_t _id;

		void appendToLog(const std::string &newLog);
	};
}
