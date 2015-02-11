#pragma once

#include "SoloBase.h"

namespace solo
{
	class Camera;
	class Transform;
	class Node;

	class RenderContext
	{
		friend class Scene;

	public:
		Node* getNode() const
		{
			return node;
		}

		Camera* getCamera() const
		{
			return camera;
		}

		Transform* getNodeTransform() const
		{
			return nodeTransform;
		}

		Transform* getCameraTransform() const
		{
			return cameraTransform;
		}

	private:
		Node* node;
		Camera* camera;
		Transform* nodeTransform;
		Transform* cameraTransform;

		RenderContext(Node* node, Transform* nodeTransform, Camera* camera, Transform* cameraTransform):
			node(node),
			camera(camera),
			nodeTransform(nodeTransform),
			cameraTransform(cameraTransform)
		{
		}

		void setNode(Node* node, Transform* nodeTransform)
		{
			this->node = node;
			this->nodeTransform = nodeTransform;
		}
	};
}