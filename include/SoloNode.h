#pragma once

namespace solo
{
	class Scene;

	// A convenient wrapper for working with components. There's no real "Node" in the engine ;)
	class Node
	{
	public:
		Node(Scene* scene, size_t nodeId);

		size_t getId() const;
		Scene* getScene() const;

		void removeAllComponents();

		template <typename T, typename... Args>
		T* addComponent(Args... args)
		{
			return scene->addComponent<T>(this->getId(), args...);
		}

		template <typename T> void removeComponent()
		{
			scene->removeComponent<T>(this->getId());
		}

		template <typename T> T* getComponent()
		{
			return scene->getComponent<T>(this->getId());
		}

		template <typename T> T* findComponent()
		{
			return scene->findComponent<T>(this->getId());
		}

	private:
		Scene* scene;
		size_t id;
	};
}