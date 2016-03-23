#pragma once

#include "SoloConfig.h"
#include "SoloFormatter.h"
#include "SoloException.h"
#include <memory>


#ifdef SL_DEBUG
#   define SL_IN_DEBUG(code) code
#else
#   define SL_IN_DEBUG(function)
#endif

#define SL_SHARED_STATIC_CAST		std::static_pointer_cast
#define SL_MAKE_SHARED				std::make_shared
#define SL_MAKE_UNIQUE              std::make_unique
#define SL_NEW_SHARED(type, ...) 	std::shared_ptr<type>(new type(__VA_ARGS__)) // TODO replace with SL_MAKE_SHARED

#define SL_NONCOPYABLE(type) \
    type(const type &other) = delete; \
    type(type &&other) = delete; \
    type &operator=(const type &other) = delete; \
    type &operator=(type &&other) = delete;

namespace solo
{
    template <typename T> using shared = std::shared_ptr<T>;
    template <typename T> using unique = std::unique_ptr<T>;
}
