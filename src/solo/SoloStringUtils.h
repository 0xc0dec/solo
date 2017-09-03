/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include <string>

namespace solo
{
    namespace stringutils
    {
        inline bool endsWith(const std::string &s, const std::string &ending)
        {
            return std::equal(ending.rbegin(), ending.rend(), s.rbegin());
        }
    }
}