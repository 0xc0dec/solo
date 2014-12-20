#ifndef __SOLO_TRANSFORM_H__
#define __SOLO_TRANSFORM_H__

#include "SoloITransform.h"
#include "SoloVector3.h"
#include "SoloQuaternion.h"
#include "SoloMatrix.h"

namespace solo
{
	class Transform : public ITransform
	{
	public:
		DECLARE_CMP_ID(Transform)
	};
}

#endif