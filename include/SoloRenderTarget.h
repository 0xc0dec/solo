#pragma once

#include "SoloBase.h"
#include "SoloRenderer.h"
#include "SoloVector2.h"

namespace solo
{
    class Texture2D;

    // TODO rename to RenderBuffer
    class RenderTarget final
    {
    public:
        RenderTarget(Renderer* renderer);
        ~RenderTarget();

        SL_NONCOPYABLE(RenderTarget)

        void bind();
        void unbind();

        void setAttachments(const std::vector<shared<Texture2D>> attachments);

        Vector2 getSize() const;

    private:
        Renderer* renderer;
        FrameBufferHandle handle;
        Vector2 size;
    };

    inline Vector2 RenderTarget::getSize() const
    {
        return size;
    }
}
