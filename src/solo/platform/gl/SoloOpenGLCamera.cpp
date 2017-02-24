/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
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
