#pragma once

namespace solo
{
	struct KnownRenderQueues
	{
		static constexpr unsigned NotRendered = 0;
		static constexpr unsigned Skyboxes = 50;
		static constexpr unsigned OpaqueObjects = 100;
	};
}
