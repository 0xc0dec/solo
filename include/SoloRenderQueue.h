#pragma once

namespace solo
{
	struct KnownRenderQueues
	{
		static constexpr unsigned NotRendered = 0;
		static constexpr unsigned CameraDefault = 100;
		static constexpr unsigned Skyboxes = 500;
		static constexpr unsigned OpaqueObjects = 1000;
	};
}
