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

#include "../../../include/Solo.h" // TODO add include directory in proj settings

using namespace solo;


class Targeter: public ComponentBase<Targeter>
{
public:
    explicit Targeter(const Node &node, Vector3 targetPos):
        ComponentBase<Targeter>(node),
        targetPos(targetPos)
    {
    }

    void init() override final
    {
        transform = node.findComponent<Transform>();
    }

    void update() override final
    {
        transform->lookAt(targetPos, Vector3::unitY());
    }

private:
    Vector3 targetPos;
    Transform *transform = nullptr;
};
