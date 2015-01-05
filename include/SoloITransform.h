#pragma once

#include "SoloIComponent.h"

namespace solo
{
	class ITransform: public IComponent
	{
	public:
		DECLARE_COMPONENT(ITransform)

		virtual void addChild(ptr<ITransform> child) = 0;
		virtual void removeChild(ptr<ITransform> child) = 0;
		virtual void removeChildren() = 0;
	};
}
