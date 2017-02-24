/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include <sstream>


#define SL_FMT(...) solo::Formatter()(__VA_ARGS__)

namespace solo
{
    class Formatter final
    {
    public:
        auto operator()() const
        {
            return buf.str();
        }

        template <class TFirst, class... TRest>
        auto operator()(TFirst first, TRest... rest)
        {
            buf << first;
            return operator()(rest...);
        }

    private:
        std::ostringstream buf;
    };
}
