#pragma once

#include "SoloBase.h"

namespace solo
{
	class Effect;

	class ResourceManager
	{
	public:
		~ResourceManager() {}

		ptr<Effect> createEffect(const std::string &vsSrc, const std::string &fsSrc);

	private:
		friend class ResourceManagerFactory;

		ResourceManager() {}

		std::list<ptr<Effect>> effects;
	};

	class ResourceManagerFactory
	{
		friend class Engine;
		static ptr<ResourceManager> create();
	};
}