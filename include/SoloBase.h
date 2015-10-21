#pragma once

#include <memory>
#include <string>
#include <functional>
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
#	define SL_STATIC_CAST		std::static_pointer_cast
#	define SL_DYNAMIC_CAST		std::dynamic_pointer_cast
#	define SL_NEW				std::make_shared
#	define SL_NEW2(type, ...)	std::shared_ptr<type>(new type(__VA_ARGS__))

	// Having this pointer means that the code owns the pointed object (just like, maybe, several other places).
	// Code that works with raw pointers doesn't claim to own the pointed object.
	template <typename T> using shared = std::shared_ptr<T>;

	extern "C" size_t getHash(const std::string &s);
}
