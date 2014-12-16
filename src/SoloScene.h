#ifndef __SOLO_SCENE_H__
#define __SOLO_SCENE_H__

#include "SoloBaseInternal.h"
#include "SoloIScene.h"

namespace solo
{
	class SceneNode;
	class IComponent;

	class Scene: public IScene
	{
	public:
		explicit Scene();

		virtual int createNode() override;

		virtual bool nodeExists(int node) override;

		virtual void addComponent(int node, ptr<IComponent> cmp) override;
		virtual ptr<IComponent> findComponent(int node, const std::string &id) override;
		virtual ptr<IComponent> getComponent(int node, const std::string &id) override;
		
		void update();

	private:
		int _nodeCounter;
		std::map<int, std::map<std::string, ptr<IComponent>>> _nodeComponents;

		void _ensureNodeExists(int node);
	};
}

#endif