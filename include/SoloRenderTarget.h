#pragma once

#include "SoloBase.h"
#include "SoloVector2.h"

namespace solo
{
    class Texture2D;
    enum class DeviceMode;

    class RenderTarget
    {
    public:
        static shared<RenderTarget> create(DeviceMode mode);

        RenderTarget(const RenderTarget &other) = delete;
        RenderTarget(RenderTarget &&other) = delete;
        RenderTarget &operator=(const RenderTarget &other) = delete;
        RenderTarget &operator=(RenderTarget &&other) = delete;
        virtual ~RenderTarget() {}

        virtual void bind() = 0;
        virtual void unbind() = 0;

        virtual void setColorAttachment(size_t index, shared<Texture2D> texture) = 0;
        virtual Vector2 getColorAttachmentSize() const = 0;
        virtual size_t getColorAttachmentCount() const = 0;
        virtual shared<Texture2D> getColorAttachment(size_t index) const = 0;

    protected:
        RenderTarget() {}
    };
}
