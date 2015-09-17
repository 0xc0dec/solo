#pragma once

namespace solo
{
	struct RenderContext
	{
		class Camera *camera;
		class Transform *cameraTransform;
		class Transform *nodeTransform;
		class Material* material = nullptr;
		class Scene* scene = nullptr;
	};
}