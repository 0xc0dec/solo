#pragma once

#include "SoloBase.h"

namespace solo
{
	class Effect
	{
	public:
		virtual ~Effect() {}

		static ptr<Effect> create(const std::string &vsSrc, const std::string &fsSrc);

		virtual void bind() = 0;

		size_t getId() const;

		std::string getLog() const;

		bool isValid() const;

	protected:
		Effect(const std::string &vsSrc, const std::string &fsSrc);

		bool _valid;
		std::string _log;
		size_t _id;

		void appendToLog(const std::string &newLog);
	};
}
