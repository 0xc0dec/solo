#pragma once

#include "TestBase.h"

class Mesh_Test: public TestBase
{
public:
    explicit Mesh_Test(Device *device)
        : TestBase(device)
    {
    }

    virtual void run() override
    {
        test_EmptyMesh_CheckDefaults();
        test_ChangePrimitiveType();
        test_ChangePartPrimitiveType();
        test_AddPart_CheckPartCount();
    }

    void test_EmptyMesh_CheckDefaults()
    {
        auto mesh = resourceManager->getOrCreateMesh();
        assert(mesh->getPrimitiveType() == MeshPrimitiveType::Triangles);
        assert(mesh->getPartCount() == 0);
        assert(mesh->getVertexFormat().getElementCount() == 0);
        assert(mesh->getVertexFormat().getVertexSize() == 0);
    }

    void test_ChangePrimitiveType()
    {
        auto mesh = resourceManager->getOrCreateMesh();
        mesh->setPrimitiveType(MeshPrimitiveType::Points);
        assert(mesh->getPrimitiveType() == MeshPrimitiveType::Points);
    }

    void test_AddPart_CheckPartCount()
    {
        auto mesh = resourceManager->getOrCreateMesh();
        mesh->addIndexedPart();
        mesh->addIndexedPart();
        assert(mesh->getPartCount() == 2);
    }

    void test_ChangePartPrimitiveType()
    {
        auto mesh = resourceManager->getOrCreateMesh();
        auto part = mesh->addIndexedPart();
        assert(part->getPrimitiveType() == MeshPrimitiveType::Triangles);
        part->setPrimitiveType(MeshPrimitiveType::Lines);
        assert(part->getPrimitiveType() == MeshPrimitiveType::Lines);
    }
};