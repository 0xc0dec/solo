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


class FrameBuffer_Test final: public TestBase
{
public:
    FrameBuffer_Test(Device *device): TestBase(device)
    {
    }

    void run() override final
    {
        test_GetSize();
    }

private:
    void test_GetSize()
    {
        auto t1 = RectTexture::create(device);
        auto t2 = RectTexture::create(device);
        t1->setData(TextureFormat::RGB, {}, 64, 64);
        t2->setData(TextureFormat::RGB, {}, 64, 64);

        auto fb = FrameBuffer::create(device);
        fb->setAttachments({ t1, t2 });

        auto size = fb->getSize();
        assert(static_cast<uint32_t>(size.x) == 64 && static_cast<uint32_t>(size.y) == 64);
    }
};
