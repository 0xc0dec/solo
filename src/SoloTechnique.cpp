#include "SoloTechnique.h"


std::shared_ptr<solo::Technique> solo::Technique::create()
{
	return ALLOC_WITH_DELETER(Technique);
}