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

#include "SoloOpenGLGraphics.h"
#include "SoloMesh.h"
#include "SoloMaterial.h"
#include "SoloFrameBuffer.h"
#include "SoloDevice.h"
#include "platform/opengl/SoloOpenGLRenderer.h"


#ifdef SL_OPENGL_RENDERER

using namespace solo;


OpenGLGraphics::OpenGLGraphics(Device *device, const FriendToken<Device> &token):
    Graphics(token),
    device(device)
{
    renderer = dynamic_cast<OpenGLRenderer *>(device->getRenderer());
}


void OpenGLGraphics::blit(Material *material, FrameBuffer *target)
{
    if (!quadMesh)
        quadMesh = Mesh::create(device, MeshPrefab::Quad);

    material->setDepthTest(false);
    material->setDepthWrite(false);
    material->setFaceCull(FaceCull::All);

    if (target)
        target->bind();

    auto viewportSize = target ? target->getSize() : device->getCanvasSize();
    renderer->setViewport(static_cast<uint32_t>(0), static_cast<uint32_t>(0),
        static_cast<uint32_t>(viewportSize.x), static_cast<uint32_t>(viewportSize.y));

    material->apply(nullptr, nullptr, nullptr);
    quadMesh->draw(material->getEffect());

    if (target)
        target->unbind();
}

#else
#   error OpenGL renderer is not supported on this platform
#endif
