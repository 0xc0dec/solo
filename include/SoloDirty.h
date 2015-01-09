#pragma once

namespace solo
{
	class Dirty
	{
	protected:
		unsigned _dirtyBits;

		Dirty() : _dirtyBits(0)
		{
		}

		bool isClean()
		{
			return _dirtyBits == 0;
		}

		template <unsigned bit>
		bool checkAndCleanBit()
		{
			auto result = isDirty<bit>();
			clean<bit>();
			return result;
		}

		template <unsigned bit>
		bool isDirty()
		{
			return (_dirtyBits & bit) != 0;
		}

		template <unsigned bit>
		void clean()
		{
			_dirtyBits &= ~bit;
		}

		template <unsigned firstBit, unsigned secondBit, unsigned... otherBits>
		void clean()
		{
			_dirtyBits &= ~firstBit;
			clean<secondBit, otherBits...>();
		}

		template <unsigned bit>
		void setDirty()
		{
			_dirtyBits |= bit;
		}

		template <unsigned firstBit, unsigned secondBit, unsigned... otherBits>
		void setDirty()
		{
			_dirtyBits |= firstBit;
			setDirty<secondBit, otherBits...>();
		}
	};
}
