#pragma once

#include "SoloBase.h"
#include "SoloComponent.h"

namespace solo
{
	class Transform: public ComponentBase<Transform>
	{
	public:
		Transform() {}
		virtual ~Transform() override {}

		void addChild(ptr<Transform> child);
		void removeChild(ptr<Transform> child);
		void removeChildren();

	private:
		ptr<Transform> _parent;
		std::list<ptr<Transform>> _children;
	};
}
