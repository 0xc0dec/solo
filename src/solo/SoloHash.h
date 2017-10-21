/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include <functional>

namespace solo
{
    inline void combineHash(size_t &seed, size_t hash)
	{
		hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hash;
	}
}
