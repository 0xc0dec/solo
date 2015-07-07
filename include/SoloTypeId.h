#pragma once

#include "SoloBase.h"

namespace solo
{
	struct TypeId
	{
		template <typename T>
		static size_t get()
		{
			static const auto id = counter++;
			return id;
		}

	private:
		static size_t counter;
	};
}
