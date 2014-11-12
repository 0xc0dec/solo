#ifndef __SOLO_I_COMPONENT_H__
#define __SOLO_I_COMPONENT_H__

#include "SoloCommon.h"
#include "SoloISceneNode.h"

namespace solo
{
	class ISceneNode;

	class IComponent
	{
	public:
		explicit IComponent(ISceneNode *node)
			: _node(node)
		{
		}

		virtual ~IComponent() {}

		virtual String id() = 0;

		virtual void update() {}
		virtual void start() {}

	protected:
		ISceneNode *_node;
	};
}

#endif
