#ifndef __SOLO_TYPE_ID_H__
#define __SOLO_TYPE_ID_H__

#include "SoloBase.h"

namespace solo
{
	template <typename T>
	struct TypeMarker
	{
		static void f() { }
	};

	template <typename T>
	size_t getTypeId()
	{
		return reinterpret_cast<size_t>(&TypeMarker<T>::f);
	}
}

#endif