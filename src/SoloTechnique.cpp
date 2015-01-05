#include "SoloTechnique.h"


std::shared_ptr<solo::Technique> solo::Technique::create()
{
	return ALLOC_WITH_DELETER(Technique);
}


unsigned solo::Technique::getPassCount() const
{
	return static_cast<unsigned>(_passes.size());
}


std::shared_ptr<solo::Pass> solo::Technique::getPass(unsigned index) const
{
	return _passes[index];
}