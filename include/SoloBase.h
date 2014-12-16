#ifndef __SOLO_BASE_H__
#define __SOLO_BASE_H__

#include <memory>
#include <string>
#include <functional>
#include <map>
#include <list>
#include <vector>
#include <cassert>
#include <algorithm>
#include <tuple>

namespace solo
{
#	define dcast		std::dynamic_pointer_cast
#	define scast		std::static_pointer_cast
#	define makeptr		std::make_shared

	template <typename T> using ptr = std::shared_ptr<T>;

	extern "C" size_t computeHash(const std::string &s);
}

#endif