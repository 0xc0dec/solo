#pragma once

#include "TestBase.h"

class BitFlags_Test : public TestBase
{
public:
	BitFlags_Test(Engine* engine): TestBase(engine)
	{
	}

	virtual void run() override
	{
		test_NewFlags_EnsureEmpty();
		test_AddFlag_Check_Remove_EnsureEmptyAgain();
		test_SetMultipleFlags_Check_AddFlag_Check();
		test_SetAll_Check();
	}

private:
	void test_SetAll_Check()
	{
		BitFlags flags;
		flags.setAll();
		assert(flags.isSet(1));
		assert(flags.isSet(2));
		assert(flags.isSet(4));
		assert(flags.isSet(8));
	}

	void test_NewFlags_EnsureEmpty()
	{
		assert(BitFlags().isEmpty());
	}

	void test_AddFlag_Check_Remove_EnsureEmptyAgain()
	{
		BitFlags flags;
		flags.add(1);
		assert(!flags.isEmpty());
		assert(flags.isSet(1));
		assert(flags.checkAndRemove(1));
		assert(!flags.isSet(1));
		assert(flags.isEmpty());
	}

	void test_SetMultipleFlags_Check_AddFlag_Check()
	{
		BitFlags flags;

		flags.set(1 | 2 | 4);
		assert(flags.isSet(1));
		assert(flags.isSet(2));
		assert(flags.isSet(4));

		flags.add(8);
		assert(flags.isSet(8));

		flags.remove(4);
		assert(!flags.isSet(4));
		assert(flags.isSet(1));
		assert(flags.isSet(2));
		assert(flags.isSet(8));
	}
};
