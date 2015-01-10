#pragma once

#include "SoloBase.h"

namespace solo
{
	class Camera;
	class Transform;

	struct RenderContext
	{
		size_t node;
		ptr<Camera> camera;
		ptr<Transform> nodeTransform;
		ptr<Transform> cameraTransform;

		RenderContext(size_t node, ptr<Camera> camera, ptr<Transform> nodeTransform, ptr<Transform> cameraTransform):
			node(node),
			camera(camera),
			nodeTransform(nodeTransform),
			cameraTransform(cameraTransform)
		{
		}
	};
}