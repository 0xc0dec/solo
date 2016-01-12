#include "SoloOpenGLMaterial.h"
#include "SoloOpenGLHelper.h"
#include <GL/glew.h>

using namespace solo;


OpenGLMaterial::OpenGLMaterial(shared<Effect> effect):
    Material(effect)
{
}


void OpenGLMaterial::applyState()
{
    switch (polygonFace)
    {
    case PolygonFace::All:
        glDisable(GL_CULL_FACE);
        break;
    case PolygonFace::CW:
        glFrontFace(GL_CW);
        glEnable(GL_CULL_FACE);
        break;
    case PolygonFace::CCW:
        glFrontFace(GL_CCW);
        glEnable(GL_CULL_FACE);
        break;
    }

    if (depthTest)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    auto depthFunc = OpenGLHelper::convertToGLDepthFunc(depthPassFunc);
    glDepthFunc(depthFunc);

    glDepthMask(depthWrite ? GL_TRUE : GL_FALSE);
}
