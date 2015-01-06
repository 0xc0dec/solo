#include "SoloTechnique.h"

using namespace solo;


std::shared_ptr<Technique> Technique::create()
{
	return NEW2(Technique);
}


unsigned Technique::getPassCount() const
{
	return static_cast<unsigned>(_passes.size());
}


std::shared_ptr<Pass> Technique::getPass(unsigned index) const
{
	return _passes[index];
}