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
#include "../common/Shaders.h"
#include "SpawnedObject.h"

using namespace solo;


class Spawner final: public ComponentBase<Spawner>
{
public:
    explicit Spawner(const Node &node, sptr<Mesh> mesh):
        ComponentBase<Spawner>(node),
        device(node.getScene()->getDevice()),
        scene(device->getScene()),
        transform(node.findComponent<Transform>()),
        mesh(mesh)
    {
    }

    void init() override final
    {
        effect = Effect::create(device, commonShaders.vertex.basic, commonShaders.fragment.color);
    }

    void update() override final
    {
        if (device->isKeyPressed(KeyCode::Space, true))
            spawn();
    }

private:
    void spawn()
    {
        auto initialPos = transform->getLocalPosition() + transform->getLocalForward() * 2;
        auto initialRotation = transform->getLocalRotation();
        scene->createNode()->addComponent<SpawnedObject>(effect, mesh, initialPos, initialRotation);
    }

    Device *device = nullptr;
    Scene *scene = nullptr;
    Transform *transform = nullptr;
    sptr<Material> material;
    sptr<Mesh> mesh;
    sptr<Effect> effect;
};
