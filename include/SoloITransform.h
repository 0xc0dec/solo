#ifndef __SOLO_I_TRANSFORM_H__
#define __SOLO_I_TRANSFORM_H__

#include "SoloIComponent.h"

namespace solo
{
	class ITransform: public IComponent
	{
	public:
		virtual void addChild(ptr<ITransform> child) = 0;
		virtual void removeChild(ptr<ITransform> child) = 0;
		virtual void removeChildren() = 0;
	};
}

#endif