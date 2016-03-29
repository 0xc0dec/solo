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
        auto mesh = Mesh::create();
        assert(mesh->getPrimitiveType() == PrimitiveType::Triangles);
        assert(mesh->getPartCount() == 0);
    }

    void test_ChangePrimitiveType()
    {
        auto mesh = Mesh::create();
        mesh->setPrimitiveType(PrimitiveType::Points);
        assert(mesh->getPrimitiveType() == PrimitiveType::Points);
    }

    void test_AddIndex_CheckIndexCount()
    {
        auto mesh = Mesh::create();
        mesh->addPart(nullptr, 0);
        mesh->addPart(nullptr, 0);
        assert(mesh->getPartCount() == 2);
    }
};