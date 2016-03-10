#pragma once

#include "TestBase.h"


class Mesh_Test: public TestBase
{
public:
    explicit Mesh_Test(Device *device): TestBase(device)
    {
    }

    virtual void run() override
    {
        test_EmptyMesh_CheckDefaults();
        test_ChangePrimitiveType();
        test_AddIndex_CheckIndexCount();
    }

    void test_EmptyMesh_CheckDefaults()
    {
        auto mesh = resourceManager->getOrCreateMesh();
        assert(mesh->getPrimitiveType() == PrimitiveType::Triangles);
        assert(mesh->getIndexCount() == 0);
    }

    void test_ChangePrimitiveType()
    {
        auto mesh = resourceManager->getOrCreateMesh();
        mesh->setPrimitiveType(PrimitiveType::Points);
        assert(mesh->getPrimitiveType() == PrimitiveType::Points);
    }

    void test_AddIndex_CheckIndexCount()
    {
        auto mesh = resourceManager->getOrCreateMesh();
        mesh->addIndex(nullptr, 0);
        mesh->addIndex(nullptr, 0);
        assert(mesh->getIndexCount() == 2);
    }
};