#pragma once

namespace solo
{
	class Node;
	class MaterialPass;

	class RenderContext
	{
	public:
		RenderContext() :
			node(nullptr),
			camera(nullptr),
			pass(nullptr)
		{
		}

		void setNode(Node* node)
		{
			this->node = node;
		}

		Node* getNode() const
		{
			return node;
		}

		void setCameraNode(Node* node)
		{
			this->camera = node;
		}

		Node* getCameraNode() const
		{
			return camera;
		}

		void setPass(MaterialPass* pass)
		{
			this->pass = pass;
		}

		MaterialPass* getPass() const
		{
			return pass;
		}

	private:
		Node* node;
		Node* camera;
		MaterialPass* pass;
	};
}