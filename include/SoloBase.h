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

#define SL_NONCOPYABLE(type) \
    type(const type &other) = delete; \
    type(type &&other) = delete; \
    type &operator=(const type &other) = delete; \
    type &operator=(type &&other) = delete;

namespace solo
{
    template <typename T> using sptr = std::shared_ptr<T>;
    template <typename T> using uptr = std::unique_ptr<T>;
}
