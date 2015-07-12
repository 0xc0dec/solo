#pragma once

namespace solo
{
	class Node;
	class Material;
	class Scene;

	class RenderContext
	{
	public:
		RenderContext() {}

		void setScene(Scene* scene)
		{
			this->scene = scene;
		}

		Scene* getScene() const
		{
			return this->scene;
		}

		void setNodeId(size_t nodeId)
		{
			this->nodeId = nodeId;
		}

		size_t getNodeId() const
		{
			return nodeId;
		}

		void setCameraNodeId(size_t cameraNodeId)
		{
			this->cameraNodeId = cameraNodeId;
		}

		size_t getCameraNodeId() const
		{
			return cameraNodeId;
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
		size_t nodeId = 0;
		size_t cameraNodeId = 0;
		Scene* scene = nullptr; // to be sure that in the future when we (possibly) have multiple scenes, the whole thing won't break apart.
		Material* material = nullptr;
	};
}