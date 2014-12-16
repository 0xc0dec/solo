#ifndef __SOLO__GAME_OBJECT_H__
#define	__SOLO__GAME_OBJECT_H__

#include "SoloBaseInternal.h"
#include "SoloISceneNode.h"

namespace solo
{
	class SceneNode : public ISceneNode
	{
	public:
		explicit SceneNode(const std::string &name);
		
		virtual std::string name() const override
		{
			return _name;
		}

		virtual void addComponent(ptr<IComponent> cmp) override;
		virtual ptr<IComponent> findComponent(const std::string &id) override;
		virtual ptr<IComponent> getComponent(const std::string &id) override;

		std::map<std::string, ptr<IComponent>> components() const override
		{
			return _components;
		}

		void update();

	protected:
		std::string _name;
		std::map<std::string, ptr<IComponent>> _components;
		std::list<ptr<IComponent>> _newComponents;
	};
}

#endif
