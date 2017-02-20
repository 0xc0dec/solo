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

#include "SoloMesh.h"
#include "SoloEffect.h"
#include "SoloDevice.h"
#include "platform/opengl/SoloOpenGLMesh.h"
#include "platform/null/SoloNullMesh.h"
#include <algorithm>

using namespace solo;


auto Mesh::create(Device *device) -> sptr<Mesh>
{
    switch (device->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<gl::Mesh>(device);
#endif
        default:
            return std::make_shared<NullMesh>();
    }
}


auto Mesh::createFromPrefab(Device *device, MeshPrefab prefab) -> sptr<Mesh>
{
    switch (device->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<gl::Mesh>(device, prefab);
#endif
        default:
            return std::make_shared<NullMesh>();
    }
}


auto Mesh::createFromData(Device *device, MeshData *data) -> sptr<Mesh>
{
    switch (device->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<gl::Mesh>(device, data);
#endif
        default:
            return std::make_shared<NullMesh>();
    }
}


void Mesh::initQuadMesh()
{
    float data[] =
    {
        -1, -1, 0,  0, 0, -1,   0, 0,
        -1, 1, 0,   0, 0, -1,   0, 1,
        1, 1, 0,    0, 0, -1,   1, 1,
        -1, -1, 0,  0, 0, -1,   0, 0,
        1, 1, 0,    0, 0, -1,   1, 1,
        1, -1, 0,   0, 0, -1,   1, 0
    };

    VertexBufferLayout layout;
    layout.add(VertexBufferLayoutSemantics::Position, 3);
    layout.add(VertexBufferLayoutSemantics::Normal, 3);
    layout.add(VertexBufferLayoutSemantics::TexCoord0, 2);

    addVertexBuffer(layout, data, 6);
    setPrimitiveType(PrimitiveType::Triangles);
}


void Mesh::initCubeMesh()
{
    float positionData[] =
    {
        -1, -1, 1,  0, 0,    -1, 1, 1,  0, 1,     1, 1, 1,   1, 1,     1, -1, 1,   1, 0,
        -1, -1, -1, 0, 0,    -1, 1, -1, 0, 1,     -1, 1, 1,  1, 1,     -1, -1, 1,  1, 0,
        1, -1, -1,  0, 0,    1, 1, -1,  0, 1,     -1, 1, -1, 1, 1,     -1, -1, -1, 1, 0,
        1, -1, 1,   0, 0,    1, 1, 1,   0, 1,     1, 1, -1,  1, 1,     1, -1, -1,  1, 0,
        -1, 1, 1,   0, 0,    -1, 1, -1, 0, 1,     1, 1, -1,  1, 1,     1, 1, 1,    1, 0,
        -1, -1, -1, 0, 0,    -1, -1, 1, 0, 1,     1, -1, 1,  1, 1,     1, -1, -1,  1, 0
    };
    uint16_t indexData[] =
    {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7,
        8, 9, 10,
        8, 10, 11,
        12, 13, 14,
        12, 14, 15,
        16, 17, 18,
        16, 18, 19,
        20, 21, 22,
        20, 22, 23
    };

    VertexBufferLayout layout;
    layout.add(VertexBufferLayoutSemantics::Position, 3);
    layout.add(VertexBufferLayoutSemantics::TexCoord0, 2);
    addVertexBuffer(layout, positionData, 24);
    addPart(indexData, 36);
    setPrimitiveType(PrimitiveType::Triangles);
}
