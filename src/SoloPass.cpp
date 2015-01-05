#include "SoloPass.h"


std::shared_ptr<solo::Pass> solo::Pass::create()
{
	return ALLOC_WITH_DELETER(Pass);
}


void solo::Pass::bind()
{
}


void solo::Pass::unbind()
{
}