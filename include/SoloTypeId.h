#pragma once

#include "SoloBase.h"

namespace solo
{
	template <typename T>
	struct TypeId
	{
		static void f() { }
	};

	template <class T>
	size_t getTypeId()
	{
		return reinterpret_cast<size_t>(&TypeId<T>::f);
	}
}
