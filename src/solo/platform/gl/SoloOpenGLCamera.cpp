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


void gl::Camera::beginFrame() const
{
    renderer->setViewport(
        static_cast<uint32_t>(viewport.x),
        static_cast<uint32_t>(viewport.y),
        static_cast<uint32_t>(viewport.z),
        static_cast<uint32_t>(viewport.w));

    renderer->setDepthWrite(true);
    renderer->setDepthTest(true);
    renderer->clear(clearFlags.color, clearFlags.depth, clearColor.x, clearColor.y, clearColor.z, clearColor.w);
}

#endif
