#pragma once

namespace solo
{
	class Node;
	class Material;

	class RenderContext
	{
	public:
		RenderContext() :
			node(nullptr),
			camera(nullptr)
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

		void setMaterial(Material* material)
		{
			this->material = material;
		}

		Material* getMaterial() const
		{
			return material;
		}

	private:
		Node* node;
		Node* camera;
		Material* material;
	};
}