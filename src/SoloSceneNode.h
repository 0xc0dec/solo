#ifndef __SOLO__GAME_OBJECT_H__
#define	__SOLO__GAME_OBJECT_H__

#include "SoloISceneNode.h"

namespace solo
{
	class SceneNode : public ISceneNode
	{
	public:
		SceneNode(const str &name);
		
		virtual str name() const override { return _name; }

	protected:
		str _name;
	};
}

#endif
