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
#include <sstream>
#include "SoloFormatter.h"

namespace solo
{
#	define PTR_DCAST	std::dynamic_pointer_cast
#	define PTR_SCAST	std::static_pointer_cast
#	define NEW			std::make_shared

	template <typename T> using ptr = std::shared_ptr<T>;

	extern "C" size_t getHash(const std::string &s);
}

#endif