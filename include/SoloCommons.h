#ifndef __SOLO_COMMONS_H__
#define __SOLO_COMMONS_H__

#include <memory>
#include <string>

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
}

#endif