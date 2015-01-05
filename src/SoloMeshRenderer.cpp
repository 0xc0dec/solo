#include "SoloMeshRenderer.h"


std::shared_ptr<solo::MeshRenderer> solo::MeshRenderer::create()
{
	return ALLOC_WITH_DELETER(MeshRenderer);
}