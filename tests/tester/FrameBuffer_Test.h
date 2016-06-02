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
        test_AttachmentsOfDifferentSizes();
        test_GetSize();
    }

private:
    void test_GetSize()
    {
        auto t1 = RectTexture::create();
        auto t2 = RectTexture::create();
        t1->setData(TextureFormat::RGB, {}, 64, 64);
        t2->setData(TextureFormat::RGB, {}, 64, 64);

        auto fb = FrameBuffer::create();
        fb->setAttachments({ t1, t2 });

        auto size = fb->getSize();
        assert(static_cast<uint32_t>(size.x) == 64 && static_cast<uint32_t>(size.y) == 64);
    }

    void test_AttachmentsOfDifferentSizes()
    {
        auto fb = FrameBuffer::create();
        auto t1 = RectTexture::create();
        auto t2 = RectTexture::create();
        t1->setData(TextureFormat::RGB, {}, 64, 64);
        t2->setData(TextureFormat::RGB, {}, 16, 16);

        assertThrows<InvalidInputException>([&]()
        {
            fb->setAttachments({ t1, t2 });
        }, "Frame buffer attachments must have the same size");
    }
};
