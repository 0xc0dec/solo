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
        test_GetInexistentColorAttachment_EnsureThrows();
        test_SetColorAttachment_EnsureSet();
        test_SetAttachmentsOfDifferentSizes_EnsureThrows();
    }

private:
    void test_GetInexistentColorAttachment_EnsureThrows()
    {
        assertThrows<EngineException>([&]()
        {
            auto rt = resourceManager->getOrCreateRenderTarget("1");
            rt->getColorAttachment(0);
        }, "Invalid color attachment index 0");
    }

    void test_SetColorAttachment_EnsureSet()
    {
        auto rt = resourceManager->getOrCreateRenderTarget("2");
        assert(rt->getColorAttachmentCount() == 0);

        auto rtt = resourceManager->getOrCreateTexture2D("1");
        rtt->setData(ColorFormat::RGB, {}, 64, 64);
        rt->setColorAttachment(0, rtt);
        assert(rt->getColorAttachmentCount() == 1);
        assert(rt->getColorAttachment(0) == rtt);
    }

    void test_SetAttachmentsOfDifferentSizes_EnsureThrows()
    {
        auto rt = resourceManager->getOrCreateRenderTarget("3");
        auto rtt1 = resourceManager->getOrCreateTexture2D("2");
        auto rtt2 = resourceManager->getOrCreateTexture2D("3");
        rtt1->setData(ColorFormat::RGB, {}, 64, 64);
        rtt2->setData(ColorFormat::RGB, {}, 16, 16);

        rt->setColorAttachment(0, rtt1);
        assertThrows<EngineException>([&]()
        {
            rt->setColorAttachment(1, rtt2);
        }, "The new color attachment size differs from that of already set attachments");
    }
};
