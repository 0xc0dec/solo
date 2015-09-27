#pragma once

namespace solo
{
	class Flags
	{
	public:
		Flags() : flags(0)
		{
		}

		bool anySet() const
		{
			return flags != 0;
		}

		template <unsigned bit>
		bool checkAndUnset() const
		{
			auto result = isSet<bit>();
			clean<bit>();
			return result;
		}

		template <unsigned bit>
		bool isSet() const
		{
			return (flags & bit) != 0;
		}

		template <unsigned bit>
		void clean() const
		{
			flags &= ~bit;
		}

		template <unsigned bit1, unsigned bit2, unsigned... bitN>
		void clean() const
		{
			flags &= ~bit1;
			clean<bit2, bitN...>();
		}

		template <unsigned bit>
		void set() const
		{
			flags |= bit;
		}

		template <unsigned firstBit, unsigned secondBit, unsigned... otherBits>
		void set() const
		{
			flags |= firstBit;
			set<secondBit, otherBits...>();
		}

	private:
		mutable unsigned flags;
	};
}
