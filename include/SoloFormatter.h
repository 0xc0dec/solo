#ifndef __SOLO_FORMATTER_H__
#define __SOLO_FORMATTER_H__

#include <sstream>

namespace solo
{
	class Formatter
	{
	public:
		std::string operator()()
		{
			return _buf.str();
		}

		template <class TFirst, class... TRest>
		std::string operator()(TFirst first, TRest... rest)
		{
			_buf << first;
			return operator()(rest...);
		}

	private:
		std::ostringstream _buf;
	};
}

#define FORMAT(...) solo::Formatter()(__VA_ARGS__)

#endif