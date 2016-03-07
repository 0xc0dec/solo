#pragma once

#include "SoloFormatter.h"
#include "SoloException.h"
#include <memory>
#include <string>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <list>
#include <vector>
#include <cassert>


#ifndef NDEBUG
#   define SL_DEBUG
#endif

#define SL_MAX_TEXTURES 1024
#define SL_MAX_FRAME_BUFFERS 1024
#define SL_MAX_VERTEX_BUFFERS 1024
#define SL_MAX_INDEX_BUFFERS 1024
#define SL_MAX_PROGRAMS 1024
#define SL_MAX_VERTEX_OBJECTS 1024

#ifdef SL_DEBUG
// TODO maybe turn into GL_ASSERT or rename to SL_DEBUG_THROW
#   define SL_THROW_IF(condition, exceptionType, exceptionMessage) \
        { \
            if (condition) \
                SL_THROW_FMT(exceptionType, exceptionMessage); \
        }
#   define SL_ASSERT(condition) assert(condition);
#   define SL_MAYBE(code) code;
#else
#   define SL_THROW_IF(condition, exceptionType, exceptionMessage)
#   define SL_ASSERT(condition, msg)
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
