#ifndef __SOLO_COMMONS_H__
#define __SOLO_COMMONS_H__

#include <memory>
#include <string>
#include <functional>
#include <map>

#define castDynamic std::dynamic_pointer_cast
#define castStatic std::static_pointer_cast
#define makePtr std::make_shared

namespace solo
{
	template <typename T> using sptr = std::shared_ptr<T>;

	typedef float			f32;
	typedef double			f64;

	typedef int				s32;
	typedef unsigned int	u32;

	typedef long			s64;
	typedef unsigned long	u64;

	typedef char			c8;

	typedef std::string		str;
	typedef std::wstring	wstr;

	extern "C" size_t computeHash(const str &s);
}

#endif