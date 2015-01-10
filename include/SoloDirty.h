#pragma once

namespace solo
{
	// TODO do something with constness. Currently all methods are constant
	// to allow usage in constant methods of child classes.
	class Dirty
	{
	protected:
		mutable unsigned _dirtyBits;

		Dirty() : _dirtyBits(0)
		{
		}

		bool isClean() const
		{
			return _dirtyBits == 0;
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
			return (_dirtyBits & bit) != 0;
		}

		template <unsigned bit>
		void clean() const
		{
			_dirtyBits &= ~bit;
		}

		template <unsigned firstBit, unsigned secondBit, unsigned... otherBits>
		void clean() const
		{
			_dirtyBits &= ~firstBit;
			clean<secondBit, otherBits...>();
		}

		template <unsigned bit>
		void setDirty() const
		{
			_dirtyBits |= bit;
		}

		template <unsigned firstBit, unsigned secondBit, unsigned... otherBits>
		void setDirty() const
		{
			_dirtyBits |= firstBit;
			setDirty<secondBit, otherBits...>();
		}
	};
}
