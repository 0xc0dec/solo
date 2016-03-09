#pragma once

#include "SoloConfig.h"
#include "SoloFormatter.h"
#include "SoloException.h"
#include <memory>


#ifdef SL_DEBUG
#   define SL_MAYBE(code) code;
#else
#   define SL_MAYBE(function)
#endif

#define SL_SHARED_STATIC_CAST		std::static_pointer_cast
#define SL_MAKE_SHARED				std::make_shared
#define SL_NEW_SHARED(type, ...) 	std::shared_ptr<type>(new type(__VA_ARGS__))

#define SL_NONCOPYABLE(type) \
    type(const type &other) = delete; \
    type(type &&other) = delete; \
    type &operator=(const type &other) = delete; \
    type &operator=(type &&other) = delete;

namespace solo
{
    // The presense of this pointer indicates that the surrounding code owns the pointed object OR shares ownership with others.
    // Code that works with raw pointers doesn't claim to own the pointed object.
    template <typename T> using shared = std::shared_ptr<T>;
}
