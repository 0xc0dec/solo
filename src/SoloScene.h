#ifndef __SOLO_SCENE_H__
#define __SOLO_SCENE_H__

#include "SoloBaseInternal.h"
#include "SoloIScene.h"

namespace solo
{
	class IComponent;

	class Scene: public IScene
	{
	public:
		explicit Scene();

		virtual size_t createNode() override;
		virtual size_t createNodeWithTransform() override;

		virtual bool nodeExists(size_t node) override;

		virtual void addComponent(size_t node, ptr<IComponent> cmp) override;
		virtual ptr<IComponent> findComponent(size_t node, size_t typeId) override;
		virtual ptr<IComponent> getComponent(size_t node, size_t typeId) override;

		void addSystem(ptr<ISystem> system, size_t targetComponentTypeId) override;
		void removeSystem(ptr<ISystem> system) override;
		bool systemAlreadyAdded(ptr<ISystem> system) override;

		void update();

	private:
		int _nodeCounter;

		typedef std::map<size_t, std::map<size_t, ptr<IComponent>>> NodeComponents;
		typedef std::list<std::pair<size_t, ptr<ISystem>>> Systems; // { nodeId: { componentTypeId: component } }

		NodeComponents _nodeComponents;
		Systems _systems;

		void _ensureNodeExists(size_t node);
		Systems::iterator _findSystem(ptr<ISystem> system);
		void _ensureSystemNotAdded(ptr<ISystem> system);
	};
}

#endif