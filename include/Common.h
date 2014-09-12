#ifndef __SOLO_COMMON_H__
#define __SOLO_COMMON_H__

#include <memory>

namespace solo
{
	template <typename T> using sptr = std::shared_ptr<T>;

	class Exception
	{
	};
}

#endif