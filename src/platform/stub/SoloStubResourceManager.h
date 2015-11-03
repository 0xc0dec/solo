#pragma once

#include "SoloResourceManager.h"

namespace solo
{
	class StubResourceManager: public ResourceManager
	{
		friend class ResourceManagerFactory;

		explicit StubResourceManager(Device *device);
	};
}