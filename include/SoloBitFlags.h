#pragma once

namespace solo
{
	class BitFlags
	{
	public:
		BitFlags() {}

		bool empty() const
		{
			return flags == 0;
		}

		bool checkAndUnset(unsigned flags)
		{
			auto result = isSet(flags);
			clean(flags);
			return result;
		}

		bool isSet(unsigned flags) const
		{
			return (flags & flags) != 0;
		}

		void clean(unsigned flags)
		{
			this->flags &= ~flags;
		}

		void set(unsigned flags)
		{
			this->flags |= flags;
		}

	private:
		unsigned flags = 0;
	};
}
