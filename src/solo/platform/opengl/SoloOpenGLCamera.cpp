#include "SoloOpenGLCamera.h"
#include <GL/glew.h>

using namespace solo;


OpenGLCamera::OpenGLCamera(const Node& node):
    Camera(node)
{
}


void OpenGLCamera::applyViewport(float x, float y, float width, float height) const
{
    glViewport(x, y, width, height);
}


void OpenGLCamera::applyDepthState(bool test, bool write) const
{
    write ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    glDepthMask(test ? GL_TRUE : GL_FALSE);
}


void OpenGLCamera::clear(bool color, bool depth, float r, float g, float b, float a) const
{
    if (color)
        glClearColor(r, g, b, a);
    GLbitfield flags = (color ? GL_COLOR_BUFFER_BIT : 0) | (depth ? GL_DEPTH_BUFFER_BIT : 0);
    glClear(flags);
}
