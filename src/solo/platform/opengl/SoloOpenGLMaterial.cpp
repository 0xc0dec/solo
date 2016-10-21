#include "SoloOpenGLMaterial.h"
#include "SoloDevice.h"
#include "SoloOpenGLRenderer.h"

using namespace solo;


OpenGLMaterial::OpenGLMaterial(sptr<Effect> effect):
    Material(effect)
{
    renderer = dynamic_cast<OpenGLRenderer*>(Device::get()->getRenderer());
}


void OpenGLMaterial::applyState()
{
    renderer->setFaceCull(faceCull);
    renderer->setPolygonMode(polygonMode);
    renderer->setDepthTest(depthTest);
    renderer->setDepthWrite(depthWrite);
    renderer->setDepthFunction(depthFunc);
    renderer->setBlend(transparent);
    renderer->setBlendFactor(srcBlendFactor, dstBlendFactor);
}
