#ifndef __SOLO__GAME_OBJECT_H__
#define	__SOLO__GAME_OBJECT_H__

#include "SoloCommonsInternal.h"
#include "SoloISceneNode.h"

namespace solo
{
	class SceneNode : public ISceneNode
	{
	public:
		explicit SceneNode(const str &name);
		
		virtual str name() const override
		{
			return _name;
		}

		virtual void addComponent(sptr<IComponent> cmp) override;
		virtual sptr<IComponent> findComponent(const str &id) override;
		virtual sptr<IComponent> getComponent(const str &id) override;

		std::map<str, sptr<IComponent>> components() const override
		{
			return _components;
		}

		void update();

	protected:
		str _name;
		std::map<str, sptr<IComponent>> _components;
		std::list<sptr<IComponent>> _newComponents;
	};
}

#endif
