/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLCamera.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloDevice.h"
#include "SoloOpenGLRenderer.h"

using namespace solo;


gl::Camera::Camera(const Node &node):
    solo::Camera(node)
{
    renderer = dynamic_cast<Renderer *>(node.getScene()->getDevice()->getRenderer());
}


#endif
