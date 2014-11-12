#ifndef __SOLO__GAME_OBJECT_H__
#define	__SOLO__GAME_OBJECT_H__

#include "SoloCommonInternal.h"
#include "SoloISceneNode.h"

namespace solo
{
	class SceneNode : public ISceneNode
	{
	public:
		explicit SceneNode(const String &name);
		
		virtual String name() const override
		{
			return _name;
		}

		virtual void addComponent(ptr<IComponent> cmp) override;
		virtual ptr<IComponent> findComponent(const String &id) override;
		virtual ptr<IComponent> getComponent(const String &id) override;

		Map<String, ptr<IComponent>> components() const override
		{
			return _components;
		}

		void update();

	protected:
		String _name;
		Map<String, ptr<IComponent>> _components;
		List<ptr<IComponent>> _newComponents;
	};
}

#endif
