#pragma once

#include "SoloBase.h"

namespace solo
{
	class Camera;
	class Transform;

	class RenderContext
	{
		friend class Scene;

	public:
		size_t getNode() const
		{
			return node;
		}

		ptr<Camera> getCamera() const
		{
			return camera;
		}

		ptr<Transform> getNodeTransform() const
		{
			return nodeTransform;
		}

		ptr<Transform> getCameraTransform() const
		{
			return cameraTransform;
		}

	private:
		size_t node;
		ptr<Camera> camera;
		ptr<Transform> nodeTransform;
		ptr<Transform> cameraTransform;

		RenderContext(size_t node, ptr<Transform> nodeTransform, ptr<Camera> camera, ptr<Transform> cameraTransform) :
			node(node),
			camera(camera),
			nodeTransform(nodeTransform),
			cameraTransform(cameraTransform)
		{
		}

		void setNode(size_t node, ptr<Transform> nodeTransform)
		{
			this->node = node;
			this->nodeTransform = nodeTransform;
		}
	};
}