#pragma once

namespace solo
{
	class Dirty
	{
	protected:
		int _dirtyBits;

		Dirty() : _dirtyBits(0)
		{
		}

		bool isClean()
		{
			return _dirtyBits == 0;
		}

		template <int bit>
		bool checkBitAndClean()
		{
			auto result = isDirty<bit>();
			clean<bit>();
			return result;
		}

		template <int bit>
		bool isDirty()
		{
			return (_dirtyBits & bit) != 0;
		}

		template <int bit>
		void clean()
		{
			_dirtyBits &= ~bit;
		}

		template <int bit>
		void setDirty()
		{
			_dirtyBits |= bit;
		}

		template <int firstBit, int secondBit, int... otherBits>
		void setDirty()
		{
			_dirtyBits |= firstBit;
			setDirty<secondBit, otherBits...>();
		}
	};
}
