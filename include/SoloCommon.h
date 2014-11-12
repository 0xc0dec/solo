#ifndef __SOLO_COMMON_H__
#define __SOLO_COMMON_H__

#include <memory>
#include <string>
#include <functional>
#include <map>
#include <list>
#include <vector>
#include <cassert>
#include <algorithm>


namespace solo
{
#	define castDynamic	std::dynamic_pointer_cast
#	define castStatic	std::static_pointer_cast
#	define makePtr		std::make_shared

#	define List			std::list
#	define Map			std::map
#	define Array		std::vector
#	define Pair			std::pair

	template <typename T> using ptr = std::shared_ptr<T>;

	typedef float			f32;
	typedef double			f64;

	typedef int				s32;
	typedef unsigned int	u32;

	typedef long			s64;
	typedef unsigned long	u64;

	typedef char			c8;

	typedef std::string		String;
	typedef std::wstring	WideString;

	extern "C" size_t computeHash(const String &s);
}

#endif