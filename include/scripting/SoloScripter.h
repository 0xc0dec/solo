#pragma once

#include "SoloBase.h"

namespace solo
{
	class Scripter
	{
	public:
		virtual ~Scripter() {}

		virtual void execString(const std::string &script) = 0;
		virtual void execFile(const std::string &scriptFilePath) = 0;

	protected:
		Scripter() {}

	private:
		friend class ScripterFactory;

		Scripter(const Scripter &other) = delete;
		Scripter(Scripter &&other) = delete;
		Scripter &operator=(const Scripter &other) = delete;
		Scripter &operator=(Scripter &&other) = delete;
	};

	class ScripterFactory
	{
	public:
		static shared<Scripter> create();
	};
}