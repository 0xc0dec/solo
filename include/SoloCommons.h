#ifndef __SOLO_COMMON_H__
#define __SOLO_COMMON_H__

#include <memory>
#include <string>

namespace solo
{
	template <typename T> using sptr = std::shared_ptr<T>;

	class EngineException
	{
	public:
		std::string message;

		EngineException()
		{
		}

		EngineException(const std::string msg) : message(msg)
		{
		}
	};
}

#endif