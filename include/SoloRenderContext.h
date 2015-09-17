#pragma once

namespace solo
{
	class Material;
	class Scene;
	class Transform;
	class Camera;

	struct RenderContext
	{
		Camera *camera;
		Transform *cameraTransform;
		Transform *nodeTransform;
		Material* material = nullptr;
		Scene* scene = nullptr;
	};
}