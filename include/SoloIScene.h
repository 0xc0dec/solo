#ifndef __SOLO_I_SCENE_H__
#define __SOLO_I_SCENE_H__

#include "SoloBase.h"

namespace solo
{
	class IComponent;

	class IScene
	{
	public:
		virtual ~IScene() {}

		virtual int createNode() = 0;

		virtual bool nodeExists(int node) = 0;

		virtual void addComponent(int node, ptr<IComponent> cmp) = 0;
		virtual ptr<IComponent> findComponent(int node, const std::string &id) = 0;
		virtual ptr<IComponent> getComponent(int node, const std::string &id) = 0;

		template <typename TComponent> ptr<TComponent> addComponent(int node)
		{
			auto cmp = makeptr<TComponent>(this);
			auto base = scast<IComponent>(cmp);
			addComponent(node, base);
			return cmp;
		}

		template <typename TComponent> ptr<TComponent> getComponent(int node, const std::string &id)
		{
			auto cmp = getComponent(node, id);
			return dcast<TComponent>(cmp);
		}
	};
}

#endif