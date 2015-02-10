#pragma once

namespace solo
{
	// TODO do something with constness. Currently all methods are constant
	// to allow usage in constant methods of child classes.
	class Dirty
	{
	protected:
		mutable unsigned dirtyBits;

		Dirty() : dirtyBits(0)
		{
		}

		bool isDirty() const
		{
			return dirtyBits != 0;
		}

		template <unsigned bit>
		bool checkAndCleanBit() const
		{
			auto result = isDirty<bit>();
			clean<bit>();
			return result;
		}

		template <unsigned bit>
		bool isDirty() const
		{
			return (dirtyBits & bit) != 0;
		}

		template <unsigned bit>
		void clean() const
		{
			dirtyBits &= ~bit;
		}

		template <unsigned bit1, unsigned bit2, unsigned... bitN>
		void clean() const
		{
			dirtyBits &= ~bit1;
			clean<bit2, bitN...>();
		}

		template <unsigned bit>
		void setDirty() const
		{
			dirtyBits |= bit;
		}

		template <unsigned firstBit, unsigned secondBit, unsigned... otherBits>
		void setDirty() const
		{
			dirtyBits |= firstBit;
			setDirty<secondBit, otherBits...>();
		}
	};
}
