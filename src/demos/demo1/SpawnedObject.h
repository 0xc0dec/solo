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


class SpawnedObject final: public ComponentBase<SpawnedObject>
{
public:
    explicit SpawnedObject(const Node &node, sptr<Effect> effect, sptr<Mesh> mesh,
        const Vector3 &initialPos, const Quaternion &initialRotation):
        ComponentBase<SpawnedObject>(node),
        device(node.getScene()->getDevice()),
        scene(node.getScene()),
        initialPos(initialPos),
        initialRotation(initialRotation),
        mesh(mesh)
    {
        material = Material::create(device, effect);
        material->setFaceCull(FaceCull::All);
        material->setPolygonMode(PolygonMode::Wireframe);
        material->bindWorldViewProjectionMatrixParameter("worldViewProjMatrix");
        material->setVector4Parameter("color", Vector4(1, 1, 0, 1));
    }

    void setActive()
    {
        setColor(Vector4(1, 0, 0, 1));
        active = true;
        activeTimer = 0;
    }

    void init() override final
    {
        auto renderer = node.addComponent<MeshRenderer>();
        renderer->setMesh(mesh);
        renderer->setMaterial(0, material);
        auto t = node.findComponent<Transform>();
        t->setLocalScale({0.3f, 0.3f, 0.3f});
        t->setLocalPosition(initialPos);
        t->setLocalRotation(initialRotation);

        auto rigidBody = node.addComponent<RigidBody>(
            RigidBodyConstructionParameters()
            .withMass(50).withRestitution(0.5f).withFriction(0.2f)
            .withLinearDamping(0.1f).withAngularDamping(0.1f));
        rigidBody->setCollider(BoxCollider::create(Vector3::unit()));
    }

    void update() override final
    {
        if (!active)
            return;
        activeTimer += device->getTimeDelta();
        if (activeTimer >= 0.2f)
        {
            setColor(Vector4(1, 1, 0, 1));
            active = false;
        }
    }

private:
    void setColor(const Vector4 &color)
    {
        material->setVector4Parameter("color", color);
    }

    Device *device = nullptr;
    Scene *scene = nullptr;
    Vector3 initialPos;
    Quaternion initialRotation;
    sptr<Material> material;
    sptr<Mesh> mesh;
    bool active = false;
    float activeTimer = 0;
};
