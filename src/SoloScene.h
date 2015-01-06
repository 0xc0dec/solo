#pragma once

#include "SoloBaseInternal.h"
#include "SoloIScene.h"

namespace solo
{
	class IComponent;
	class ICamera;
	class Camera;

	class Scene: public IScene
	{
	public:
		static ptr<Scene> create();

		virtual size_t createEmptyNode() override;
		virtual size_t createNode() override;
		virtual bool nodeExists(size_t node) override;

		virtual ptr<ICamera> addCamera(size_t node) override;
		virtual ptr<IModelRenderer> addModelRenderer(size_t node) override;

		virtual void addComponent(size_t node, ptr<IComponent> cmp) override;
		virtual ptr<IComponent> findComponent(size_t node, size_t typeId) override;
		virtual ptr<IComponent> getComponent(size_t node, size_t typeId) override;

		void update();
		void render();

	private:
		Scene();
		virtual ~Scene() override {}

		typedef std::map<size_t, std::map<size_t, ptr<IComponent>>> NodeComponents;

		int _nodeCounter;
		ptr<Camera> _primaryCamera;
		NodeComponents _nodeComponents;

		void ensureNodeExists(size_t node);
	};
}
