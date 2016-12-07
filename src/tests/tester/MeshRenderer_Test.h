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

#include "TestBase.h"


class MeshRenderer_Test final: public TestBase
{
public:
    MeshRenderer_Test(Device *device): TestBase(device)
    {
    }

    void run() override final
    {
        material = Material::create(Effect::create(device, "1", "2"));
        mesh = Mesh::create();
        mesh->addPart(nullptr, 0);
        mesh->addPart(nullptr, 0);
        mesh->addPart(nullptr, 0);

        test_EnsureNoMaterialsAtFirst();
        test_NoMeshAfterSetUnset();
        test_SetMaterialForVariousIndexes();
        test_MaterialCountChanges();
    }

private:
    void test_MaterialCountChanges()
    {
        auto renderer = scene->createNode()->addComponent<MeshRenderer>();
        renderer->setMesh(mesh);
        renderer->setMaterial(0, material);
        renderer->setMaterial(1, material);
        assert(renderer->getMaterialCount() == 2);
        assert(renderer->getMaterial(0) == material.get());
        assert(renderer->getMaterial(1) == material.get());
        renderer->setMaterial(0, nullptr);
        assert(renderer->getMaterialCount() == 1);
        assert(renderer->getMaterial(1) == material.get());
    }

    void test_SetMaterialForVariousIndexes()
    {
        auto renderer = scene->createNode()->addComponent<MeshRenderer>();
        renderer->setMesh(mesh);
        renderer->setMaterial(0, material);
        renderer->setMaterial(1, material);
        renderer->setMaterial(2, material);
    }

    void test_EnsureNoMaterialsAtFirst()
    {
        auto renderer = scene->createNode()->addComponent<MeshRenderer>();
        assert(renderer->getMaterialCount() == 0);
    }

    void test_NoMeshAfterSetUnset()
    {
        auto renderer = scene->createNode()->addComponent<MeshRenderer>();
        renderer->setMesh(mesh);
        assert(renderer->getMesh() == mesh.get());
        assert(renderer->getMaterialCount() == 0); // materials get reset

        renderer->setMesh(nullptr);
        assert(renderer->getMesh() == nullptr);
    }

private:
    sptr<Mesh> mesh = nullptr;
    sptr<Material> material = nullptr;
};
