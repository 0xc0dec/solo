#ifndef __SOLO_I_SCENE_NODE_H__
#define __SOLO_I_SCENE_NODE_H__

#include "SoloCommon.h"
#include "SoloIComponent.h"

namespace solo
{
	class IComponent;
	
	class ISceneNode
	{
	public:
		virtual ~ISceneNode() {}
		
		virtual str name() const = 0;

		virtual void addComponent(sptr<IComponent> cmp) = 0;
		virtual sptr<IComponent> findComponent(const str &id) = 0;
		virtual sptr<IComponent> getComponent(const str &id) = 0;
		virtual std::map<str, sptr<IComponent>> components() const = 0;

		template <typename TComponent> sptr<TComponent> addComponent()
		{
			auto cmp = makePtr<TComponent>(this);
			auto base = castStatic<IComponent>(cmp);
			addComponent(base);
			return cmp;
		}

		template <typename TComponent> sptr<TComponent> getComponent(const str &id)
		{
			auto cmp = getComponent(id);
			return castDynamic<TComponent>(cmp);
		}
	};
}

#endif
