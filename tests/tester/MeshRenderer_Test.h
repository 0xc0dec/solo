#pragma once

#include "TestBase.h"

class MeshRenderer_Test : public TestBase
{
public:
    MeshRenderer_Test(Device *device) :
        TestBase(device)
    {
    }

    virtual void run() override
    {
        renderer = scene->createNode()->addComponent<MeshRenderer>();
        material = resourceManager->getOrCreateMaterial(resourceManager->getOrCreateEffect("1", "2"));
        mesh = resourceManager->getOrCreateMesh(VertexFormat());
        mesh->addIndex(MeshIndexFormat::UnsignedShort);
        mesh->addIndex(MeshIndexFormat::UnsignedShort);
        mesh->addIndex(MeshIndexFormat::UnsignedShort);

        test_EnsureNoMaterialsAtFirst();
        test_SetMesh_UnsetMesh_EnsureNoMesh();
        test_SetMaterialForVariousIndexes();
        test_SetMaterial_EnsureSetForOneIndex();
        test_UnsetMesh_SetMaterial_EnsureThrows();
        test_SetMaterial_UnsetMaterial_EnsureMaterialCountChanges();
    }

    void test_SetMaterial_UnsetMaterial_EnsureMaterialCountChanges()
    {
        renderer->setMesh(nullptr); // ensure no materials
        renderer->setMesh(mesh);
        renderer->setMaterial(0, material);
        renderer->setMaterial(1, material);
        assert(renderer->getMaterialCount() == 2);
        renderer->setMaterial(0, nullptr);
        assert(renderer->getMaterialCount() == 1);
        assert(renderer->findMaterial(0) == nullptr);
        assert(renderer->findMaterial(1) == material.get());
    }

    void test_SetMaterial_EnsureSetForOneIndex()
    {
        renderer->setMesh(mesh);
        renderer->setMaterial(0, material);
        assert(renderer->findMaterial(0) == material.get());
        assert(renderer->findMaterial(1) == nullptr);
        assert(renderer->getMaterialCount() == 1);
    }

    void test_SetMaterialForVariousIndexes()
    {
        renderer->setMesh(mesh);
        renderer->setMaterial(0, material);
        renderer->setMaterial(1, material);
        renderer->setMaterial(2, material);
        assertThrows<EngineException>([ = ]()
        {
            renderer->setMaterial(3, material);
        });
    }

    void test_EnsureNoMaterialsAtFirst()
    {
        assert(renderer->getMaterialCount() == 0);
    }

    void test_SetMesh_UnsetMesh_EnsureNoMesh()
    {
        renderer->setMesh(mesh);
        assert(renderer->getMesh() == mesh.get());
        assert(renderer->getMaterialCount() == 0); // materials get reset

        renderer->setMesh(nullptr);
        assert(renderer->getMesh() == nullptr);
    }

    void test_UnsetMesh_SetMaterial_EnsureThrows()
    {
        renderer->setMesh(mesh);
        renderer->setMaterial(0, material);
        renderer->setMesh(nullptr);
        assertThrows<EngineException>([ = ]()
        {
            renderer->setMaterial(0, material);
        });
    }

private:
    shared<Mesh> mesh = nullptr;
    shared<Material> material = nullptr;
    MeshRenderer *renderer = nullptr;
};
