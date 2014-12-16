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
		
		virtual std::string name() const = 0;

		virtual void addComponent(ptr<IComponent> cmp) = 0;
		virtual ptr<IComponent> findComponent(const std::string &id) = 0;
		virtual ptr<IComponent> getComponent(const std::string &id) = 0;
		virtual std::map<std::string, ptr<IComponent>> components() const = 0;

		template <typename TComponent> ptr<TComponent> addComponent()
		{
			auto cmp = makeptr<TComponent>(this);
			auto base = scast<IComponent>(cmp);
			addComponent(base);
			return cmp;
		}

		template <typename TComponent> ptr<TComponent> getComponent(const std::string &id)
		{
			auto cmp = getComponent(id);
			return dcast<TComponent>(cmp);
		}
	};
}

#endif
