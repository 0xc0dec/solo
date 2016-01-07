#pragma once

#include <memory>
#include <string>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <list>
#include <vector>
#include "SoloFormatter.h"
#include "SoloException.h"

namespace solo
{
#	define SL_SHARED_STATIC_CAST		std::static_pointer_cast
#	define SL_MAKE_SHARED				std::make_shared
#	define SL_NEW_SHARED(type, ...)	std::shared_ptr<type>(new type(__VA_ARGS__))

#   define SL_NONCOPYABLE(type) \
        type(const type &other) = delete; \
        type(type &&other) = delete; \
        type &operator=(const type &other) = delete; \
        type &operator=(type &&other) = delete;

    // Having this pointer means that the code owns the pointed object (just like, maybe, several other places).
    // Code that works with raw pointers doesn't claim to own the pointed object.
    template <typename T> using shared = std::shared_ptr<T>;

    extern "C" size_t getHash(const std::string &s);
}
