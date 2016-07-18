#pragma once

#include "SoloConfig.h"
#include "SoloFormatter.h"
#include "SoloException.h"
#include <memory>


#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS)
#   define SL_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
#   define SL_MACOS
#elif defined(__linux__)
#   define SL_LINUX
#endif

#if defined(SL_WINDOWS) || defined(SL_MACOS)
#   define SL_OPENGL_RENDERER
#endif

#if defined(SL_WINDOWS)
#   define SL_VULKAN_RENDERER
#endif

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

#define SL_FLUENT_DTO(dtoType, fields) \
    struct dtoType final \
    { \
        typedef dtoType DtoType; \
        fields \
    };

#define SL_FLUENT_DTO_FIELD(fieldType, fieldName, setterMethodName, defaultValue) \
    fieldType fieldName defaultValue; \
    auto setterMethodName(fieldType fieldName) -> DtoType& \
    { \
        this->fieldName = fieldName; \
        return *this; \
    }

namespace solo
{
    template <typename T> using sptr = std::shared_ptr<T>;
    template <typename T> using uptr = std::unique_ptr<T>;
}
