#pragma once

namespace solo
{
	class Node;
	class MaterialPass;

	class RenderContext
	{
	public:
		Node* getNode() const
		{
			return node;
		}

		Node* getCameraNode() const
		{
			return camera;
		}

		void setCurrentPass(MaterialPass* pass)
		{
			this->pass = pass;
		}

		MaterialPass* getCurrentPass() const
		{
			return pass;
		}

	private:
		friend class RenderContextFactory;

		RenderContext(Node* node, Node* camera, MaterialPass* pass):
			node(node),
			camera(camera),
			pass(pass)
		{
		}

		Node* node;
		Node* camera;
		MaterialPass* pass;
	};

	class RenderContextFactory
	{
		friend class Scene;

		static RenderContext create(Node* node, Node* camera, MaterialPass* pass)
		{
			return RenderContext(node, camera, pass);
		}
	};
}