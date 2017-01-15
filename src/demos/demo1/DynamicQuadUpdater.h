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

#pragma once

#include <Solo.h>

using namespace solo;


class DynamicQuadUpdater: public ComponentBase<DynamicQuadUpdater>
{
public:
    explicit DynamicQuadUpdater(const Node &node, std::vector<float> data, sptr<Mesh> mesh):
        ComponentBase<DynamicQuadUpdater>(node),
        device(node.getScene()->getDevice()),
        data(data),
        mesh(mesh)
    {
    }

    void update() override final
    {
        time += 2 * device->getTimeDelta();
        auto offset = 0.3f * sinf(time);
        data[2] = offset;
        data[7] = -offset;
        data[12] = offset;
        data[17] = -offset;
        mesh->updateDynamicVertexBuffer(0, 0, data.data(), 4);
    }

private:
    float time = 0;
    Device *device;
    std::vector<float> data;
    sptr<Mesh> mesh;
};
