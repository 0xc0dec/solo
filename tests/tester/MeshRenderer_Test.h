#pragma once

#include "TestBase.h"


class MeshRenderer_Test : public TestBase
{
public:
    MeshRenderer_Test(Device *device): TestBase(device)
    {
    }

    virtual void run() override
    {
        material = resourceManager->getOrCreateMaterial(resourceManager->getOrCreateEffect("1", "2"));
        mesh = resourceManager->getOrCreateMesh();
        mesh->addPart(nullptr, 0);
        mesh->addPart(nullptr, 0);
        mesh->addPart(nullptr, 0);

        test_EnsureNoMaterialsAtFirst();
        test_SetMesh_UnsetMesh_EnsureNoMesh();
        test_SetMaterialForVariousIndexes();
        test_SetMaterial_CheckCount();
        test_SetMaterial_UnsetMaterial_EnsureMaterialCountChanges();
    }

    void test_SetMaterial_UnsetMaterial_EnsureMaterialCountChanges()
    {
        auto renderer = scene->createNode()->addComponent<MeshRenderer>();
        renderer->setMesh(mesh);
        renderer->setMaterial(0, material);
        renderer->setMaterial(1, material);
        assert(renderer->getMaterialCount() == 2);
        renderer->setMaterial(0, nullptr);
        assert(renderer->getMaterialCount() == 1);
        assert(renderer->getMaterial(1) == material.get());
    }

    void test_SetMaterial_CheckCount()
    {
        auto renderer = scene->createNode()->addComponent<MeshRenderer>();
        renderer->setMesh(mesh);
        renderer->setMaterial(0, material);
        assert(renderer->getMaterial(0) == material.get());
        assert(renderer->getMaterialCount() == 1);
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

    void test_SetMesh_UnsetMesh_EnsureNoMesh()
    {
        auto renderer = scene->createNode()->addComponent<MeshRenderer>();
        renderer->setMesh(mesh);
        assert(renderer->getMesh() == mesh.get());
        assert(renderer->getMaterialCount() == 0); // materials get reset

        renderer->setMesh(nullptr);
        assert(renderer->getMesh() == nullptr);
    }

private:
    shared<Mesh> mesh = nullptr;
    shared<Material> material = nullptr;
};
