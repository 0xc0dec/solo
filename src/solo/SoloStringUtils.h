/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include "SoloCommon.h"

namespace solo
{
    namespace stringutils
    {
        inline bool endsWith(const str &s, const str &ending)
        {
            return std::equal(ending.rbegin(), ending.rend(), s.rbegin());
        }
    }
}