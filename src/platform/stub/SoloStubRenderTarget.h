#pragma once

#include "SoloRenderTarget.h"

namespace solo
{
    class StubRenderTarget: public RenderTarget
    {
    public:
        virtual void bind() override {}
        virtual void unbind() override {}

        virtual void setColorAttachment(int index, shared<Texture2D> texture) override {}

        virtual Vector2 getColorAttachmentSize() const override
        {
            return Vector2();
        }

        virtual int getColorAttachmentCount() const override
        {
            return 0;
        }

        virtual shared<Texture2D> getColorAttachment(int index) const override
        {
            return nullptr;
        }

    private:
        friend class RenderTarget;

        StubRenderTarget() {}
    };
}