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
        auto mesh = Mesh::create(device);
        assert(mesh->getPrimitiveType() == PrimitiveType::Triangles);
        assert(mesh->getPartCount() == 0);
    }

    void test_ChangePrimitiveType()
    {
        auto mesh = Mesh::create(device);
        mesh->setPrimitiveType(PrimitiveType::Points);
        assert(mesh->getPrimitiveType() == PrimitiveType::Points);
    }

    void test_IndexCount()
    {
        auto mesh = Mesh::create(device);
        mesh->addPart(nullptr, 0);
        mesh->addPart(nullptr, 0);
        assert(mesh->getPartCount() == 2);
    }
};