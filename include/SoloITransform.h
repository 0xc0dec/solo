#pragma once

#include "SoloIComponent.h"

namespace solo
{
	class ITransform: public Component<ITransform>
	{
	public:
		virtual void addChild(ptr<ITransform> child) = 0;
		virtual void removeChild(ptr<ITransform> child) = 0;
		virtual void removeChildren() = 0;
	};
}
