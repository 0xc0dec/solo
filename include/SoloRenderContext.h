#pragma once

namespace solo
{
	class Node;

	class RenderContext
	{
		friend class RenderContextFactory;

	public:
		Node* getNode() const
		{
			return node;
		}

		Node* getCameraNode() const
		{
			return camera;
		}

	private:
		Node* node;
		Node* camera;

		RenderContext(Node* node, Node* camera):
			node(node),
			camera(camera)
		{
		}
	};

	class RenderContextFactory
	{
		friend class Scene;

		static RenderContext create(Node* node, Node* camera)
		{
			return RenderContext(node, camera);
		}
	};
}