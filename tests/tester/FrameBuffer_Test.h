#pragma once

#include "TestBase.h"


class FrameBuffer_Test : public TestBase
{
public:
    FrameBuffer_Test(Device *device):
        TestBase(device)
    {
    }

    virtual void run() override
    {
        test_SetAttachmentsOfDifferentSizes_EnsureThrows();
        test_SetAttachments_GetSize();
    }

private:
    void test_SetAttachments_GetSize()
    {
        auto t1 = resourceManager->getOrCreateTexture2D("tex1");
        auto t2 = resourceManager->getOrCreateTexture2D("tex2");
        t1->setData(ColorFormat::RGB, {}, 64, 64);
        t2->setData(ColorFormat::RGB, {}, 64, 64);

        auto fb = resourceManager->getOrCreateFrameBuffer("2");
        fb->setAttachments({ t1, t2 });

        auto size = fb->getSize();
        assert(static_cast<int>(size.x) == 64 && static_cast<int>(size.y) == 64);
    }

    void test_SetAttachmentsOfDifferentSizes_EnsureThrows()
    {
        auto fb = resourceManager->getOrCreateFrameBuffer("3");
        auto t1 = resourceManager->getOrCreateTexture2D("tex3");
        auto t2 = resourceManager->getOrCreateTexture2D("tex4");
        t1->setData(ColorFormat::RGB, {}, 64, 64);
        t2->setData(ColorFormat::RGB, {}, 16, 16);

        assertThrows<InvalidInputException>([&]()
        {
            fb->setAttachments({ t1, t2 });
        }, "Frame buffer attachments must have the same size");
    }
};
