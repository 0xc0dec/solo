#ifndef __SOLO_COMMON_H__
#define __SOLO_COMMON_H__

#include <string>
#include <vector>
#include <list>
#include <memory>

namespace solo
{
    template <typename T> using sptr = std::shared_ptr<T>;
}

#endif