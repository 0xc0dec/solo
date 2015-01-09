#pragma once

#include "SoloBase.h"
#include "SoloComponent.h"

namespace solo
{
	class Transform: public ComponentBase<Transform>
	{
	public:
		virtual ~Transform() override {}
		
		static ptr<Transform> create(size_t node);

		void addChild(ptr<Transform> child);
		void removeChild(ptr<Transform> child);
		void removeChildren();

	private:
		Transform(size_t node);

		ptr<Transform> _parent;
		std::list<ptr<Transform>> _children;
	};
}
