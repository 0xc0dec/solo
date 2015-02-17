#pragma once

#include <memory>
#include <string>
#include <functional>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <vector>
#include <cassert>
#include <algorithm>
#include <tuple>
#include <sstream>
#include "SoloFormatter.h"
#include "SoloException.h"

namespace solo
{
#	define CAST_PTR_STATIC	std::static_pointer_cast
#	define NEW				std::make_shared
#	define NEW2(type, ...)	std::shared_ptr<type>(new type(__VA_ARGS__))

	// Usage of this pointer should indicate that that place owns an object this pointer points to.
	// Code that operates on raw pointers doesn't claim to own the pointed object.
	template <typename T> using shared = std::shared_ptr<T>;

	extern "C" size_t getHash(const std::string &s);
}
