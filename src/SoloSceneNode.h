#ifndef __SOLO__GAME_OBJECT_H__
#define	__SOLO__GAME_OBJECT_H__

#include "SoloISceneNode.h"

namespace solo
{
	class SceneNode : public ISceneNode
	{
	public:
		SceneNode(const str &name);
		
		virtual str name() const override
		{
			return _name;
		}

		virtual void addComponent(sptr<IComponent> cmp) override;
		virtual sptr<IComponent> findComponent(const str &id) override;
		virtual sptr<IComponent> getComponent(const str &id) override;

		void update();

	protected:
		str _name;
		dict<str, sptr<IComponent>> _components;
	};
}

#endif
