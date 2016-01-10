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
        auto mesh = resourceManager->getOrCreateMesh(VertexFormat());
        assert(mesh->getPrimitiveType() == MeshPrimitiveType::Triangles);
        assert(mesh->getPartCount() == 0);
        assert(mesh->getVertexFormat().getElementCount() == 0);
    }

    void test_ChangePrimitiveType()
    {
        auto mesh = resourceManager->getOrCreateMesh(VertexFormat());
        mesh->setPrimitiveType(MeshPrimitiveType::Points);
        assert(mesh->getPrimitiveType() == MeshPrimitiveType::Points);
    }

    void test_AddPart_CheckPartCount()
    {
        auto mesh = resourceManager->getOrCreateMesh(VertexFormat());
        mesh->addPart(MeshIndexFormat::UnsignedShort);
        mesh->addPart(MeshIndexFormat::UnsignedShort);
        assert(mesh->getPartCount() == 2);
    }

    void test_ChangePartPrimitiveType()
    {
        auto mesh = resourceManager->getOrCreateMesh(VertexFormat());
        auto part = mesh->addPart(MeshIndexFormat::UnsignedShort);
        assert(part->getPrimitiveType() == MeshPrimitiveType::Triangles);
        part->setPrimitiveType(MeshPrimitiveType::Lines);
        assert(part->getPrimitiveType() == MeshPrimitiveType::Lines);
    }
};