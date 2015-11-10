#pragma once

#include "SoloTypeId.h"
#include "SoloNode.h"
#include "SoloBitFlags.h"
#include "SoloRenderQueue.h"

namespace solo
{
	class Scene;
	struct RenderContext;

	class Component
	{
	public:
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;
		virtual ~Component() {}

		virtual size_t getTypeId() = 0;

		virtual void init() {}
		virtual void update() {}
		virtual void terminate() {}
		virtual void render(RenderContext& context) {}
		virtual void onAfterCameraRender() {}

		virtual unsigned getRenderQueue() { return KnownRenderQueues::NotRendered; }

		Node getNode() const;

		BitFlags& getTags();

	protected:
		explicit Component(const Node& node) : node(node)
		{
			tags.set(1);
		}
		
		Node node;
		BitFlags tags;
	};

	inline Node Component::getNode() const
	{
		return node;
	}

	inline BitFlags& Component::getTags()
	{
		return tags;
	}


	template <class T>
	class ComponentBase: public Component
	{
	public:
		explicit ComponentBase(const Node& node): Component(node) {}

		static size_t getId();
		virtual size_t getTypeId() override;
	};

	template <class T>
	inline size_t ComponentBase<T>::getId()
	{
		return TypeId::get<T>();
	}

	template <class T>
	inline size_t ComponentBase<T>::getTypeId()
	{
		return getId();
	}
}
