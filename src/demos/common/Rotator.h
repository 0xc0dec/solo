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

#include "../../../include/Solo.h"


class Rotator final: public solo::ComponentBase<Rotator>
{
public:
    explicit Rotator(const solo::Node &node, const std::string &space, solo::Vector3 axis):
        ComponentBase<Rotator>(node),
        device(solo::Device::get()),
        axis(axis),
        space(space)
    {
    }

    virtual void init() override final
    {
        transform = node.findComponent<solo::Transform>();
    }

    virtual void update() override final
    {
        auto angle = device->getTimeDelta();
        auto rotationSpace = solo::TransformSpace::World;
        if (space == "local")
            rotationSpace = solo::TransformSpace::Self;
        transform->rotate(axis, solo::Radian(angle), rotationSpace);
    }

private:
    solo::Transform *transform = nullptr;
    solo::Device *device;
    solo::Vector3 axis;
    std::string space;
};