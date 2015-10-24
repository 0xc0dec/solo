#pragma once

namespace solo
{
	class BitFlags
	{
	public:
		BitFlags() {}

		unsigned getRaw() const
		{
			return flags;
		}

		bool isEmpty() const
		{
			return flags == 0;
		}

		bool checkAndRemove(unsigned flags)
		{
			auto result = isSet(flags);
			remove(flags);
			return result;
		}

		bool isSet(unsigned flags) const
		{
			return (this->flags & flags) != 0;
		}

		void setAll()
		{
			flags = ~0;
		}

		void set(unsigned flags)
		{
			this->flags = flags;
		}

		void add(unsigned flags)
		{
			this->flags |= flags;
		}

		void remove(unsigned flags)
		{
			this->flags &= ~flags;
		}

		void clear()
		{
			flags = 0;
		}

	private:
		unsigned flags = 0;
	};
}
