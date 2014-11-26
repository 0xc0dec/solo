#ifndef __SOLO_I_SCENE_NODE_H__
#define __SOLO_I_SCENE_NODE_H__

#include "SoloBase.h"
#include "SoloIComponent.h"

namespace solo
{
	class IComponent;
	
	class ISceneNode
	{
	public:
		virtual ~ISceneNode() {}
		
		virtual String name() const = 0;

		virtual void addComponent(ptr<IComponent> cmp) = 0;
		virtual ptr<IComponent> findComponent(const String &id) = 0;
		virtual ptr<IComponent> getComponent(const String &id) = 0;
		virtual Map<String, ptr<IComponent>> components() const = 0;

		template <typename TComponent> ptr<TComponent> addComponent()
		{
			auto cmp = makePtr<TComponent>(this);
			auto base = castStatic<IComponent>(cmp);
			addComponent(base);
			return cmp;
		}

		template <typename TComponent> ptr<TComponent> getComponent(const String &id)
		{
			auto cmp = getComponent(id);
			return castDynamic<TComponent>(cmp);
		}
	};
}

#endif
