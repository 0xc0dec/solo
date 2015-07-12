#pragma once

#include "SoloTypeId.h"

namespace solo
{
	class RenderContext;
	class Scene;

	class Component
	{
	public:
		virtual ~Component() {}

		virtual size_t getTypeId() = 0;

		// Called right before rendering. Use this to update object position, animation or any other game logic.
		virtual void update() {}

		// Called when the component is allowed to render anything.
		virtual void render(RenderContext& context) {}

		virtual void postRender() {}

	protected:
		Component() {}

	private:
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;
	};

	template <class T>
	class ComponentBase: public Component
	{
	public:
		explicit ComponentBase(Scene* scene, size_t nodeId):
			scene(scene),
			nodeId(nodeId)
		{
		}

		static size_t getId()
		{
			return TypeId::get<T>();
		}

		virtual size_t getTypeId() override
		{
			return getId();
		}

		size_t getNodeId() const
		{
			return nodeId;
		}

	protected:
		Scene* scene;
		size_t nodeId;
	};
}
