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
			return _node;
		}

		ptr<Camera> getCamera() const
		{
			return _camera;
		}

		ptr<Transform> getNodeTransform() const
		{
			return _nodeTransform;
		}

		ptr<Transform> getCameraTransform() const
		{
			return _cameraTransform;
		}

	private:
		size_t _node;
		ptr<Camera> _camera;
		ptr<Transform> _nodeTransform;
		ptr<Transform> _cameraTransform;

		RenderContext(size_t node, ptr<Transform> nodeTransform, ptr<Camera> camera, ptr<Transform> cameraTransform) :
			_node(node),
			_camera(camera),
			_nodeTransform(nodeTransform),
			_cameraTransform(cameraTransform)
		{
		}

		void setNode(size_t node, ptr<Transform> nodeTransform)
		{
			_node = node;
			_nodeTransform = nodeTransform;
		}
	};
}