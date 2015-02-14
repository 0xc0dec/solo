#pragma once

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
#	define CAST_PTR_STATIC	std::static_pointer_cast
#	define NEW				std::make_shared
#	define NEW2(type, ...)	std::shared_ptr<type>(new type(__VA_ARGS__))

	template <typename T> using shared = std::shared_ptr<T>;

	extern "C" size_t getHash(const std::string &s);
}
