#pragma once

#include "TestBase.h"


class Mesh_Test final: public TestBase
{
public:
    explicit Mesh_Test(Device *device): TestBase(device)
    {
    }

    void run() override final
    {
        test_Defaults();
        test_ChangePrimitiveType();
        test_IndexCount();
    }

private:
    void test_Defaults()
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

    void test_IndexCount()
    {
        auto mesh = Mesh::create();
        mesh->addPart(nullptr, 0);
        mesh->addPart(nullptr, 0);
        assert(mesh->getPartCount() == 2);
    }
};