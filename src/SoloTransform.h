#ifndef __SOLO_TRANSFORM_H__
#define __SOLO_TRANSFORM_H__

#include "SoloITransform.h"

namespace solo
{
	class Transform : public ITransform
	{
	public:
		virtual void addChild(ptr<ITransform> child) override;
		virtual void removeChild(ptr<ITransform> child) override;
		virtual void removeChildren() override;

	private:
		ptr<Transform> _parent;
		std::list<ptr<Transform>> _children;
	};
}

#endif