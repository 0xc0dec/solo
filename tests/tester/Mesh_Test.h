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
        test_ChangeIndexPrimitiveType();
        test_AddIndex_CheckIndexCount();
    }

    void test_EmptyMesh_CheckDefaults()
    {
        auto mesh = resourceManager->getOrCreateMesh(VertexFormat());
        assert(mesh->getPrimitiveType() == MeshPrimitiveType::Triangles);
        assert(mesh->getIndexCount() == 0);
        assert(mesh->getVertexFormat().getElementCount() == 0);
    }

    void test_ChangePrimitiveType()
    {
        auto mesh = resourceManager->getOrCreateMesh(VertexFormat());
        mesh->setPrimitiveType(MeshPrimitiveType::Points);
        assert(mesh->getPrimitiveType() == MeshPrimitiveType::Points);
    }

    void test_AddIndex_CheckIndexCount()
    {
        auto mesh = resourceManager->getOrCreateMesh(VertexFormat());
        mesh->addIndex(MeshIndexFormat::UnsignedShort);
        mesh->addIndex(MeshIndexFormat::UnsignedShort);
        assert(mesh->getIndexCount() == 2);
    }

    void test_ChangeIndexPrimitiveType()
    {
        auto mesh = resourceManager->getOrCreateMesh(VertexFormat());
        auto index = mesh->addIndex(MeshIndexFormat::UnsignedShort);
        assert(mesh->getIndexPrimitiveType(index) == MeshPrimitiveType::Triangles);
        mesh->setIndexPrimitiveType(index, MeshPrimitiveType::Lines);
        assert(mesh->getIndexPrimitiveType(index) == MeshPrimitiveType::Lines);
    }
};