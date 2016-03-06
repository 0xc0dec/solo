#pragma once

#include "TestBase.h"


class RenderTargets_Test : public TestBase
{
public:
    RenderTargets_Test(Device *device):
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
        auto rtt1 = resourceManager->getOrCreateTexture2D("tex1");
        auto rtt2 = resourceManager->getOrCreateTexture2D("tex2");
        rtt1->setData(ColorFormat::RGB, {}, 64, 64);
        rtt2->setData(ColorFormat::RGB, {}, 64, 64);

        auto rt = resourceManager->getOrCreateRenderTarget("2");
        rt->setAttachments({ rtt1, rtt2 });

        auto size = rt->getSize();
        assert(static_cast<int>(size.x) == 64 && static_cast<int>(size.y) == 64);
    }

    void test_SetAttachmentsOfDifferentSizes_EnsureThrows()
    {
        auto rt = resourceManager->getOrCreateRenderTarget("3");
        auto rtt1 = resourceManager->getOrCreateTexture2D("tex3");
        auto rtt2 = resourceManager->getOrCreateTexture2D("tex4");
        rtt1->setData(ColorFormat::RGB, {}, 64, 64);
        rtt2->setData(ColorFormat::RGB, {}, 16, 16);

        assertThrows<InvalidInputException>([&]()
        {
            rt->setAttachments({ rtt1, rtt2 });
        }, "Frame buffer attachments must have the same size");
    }
};
