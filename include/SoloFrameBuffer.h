#pragma once

#include "SoloBase.h"
#include "SoloRenderer.h"
#include "SoloVector2.h"


namespace solo
{
    class Texture2D;

    class FrameBuffer final
    {
    public:
        FrameBuffer(Renderer* renderer);
        ~FrameBuffer();

        SL_NONCOPYABLE(FrameBuffer)

        void bind();
        void unbind();

        void setAttachments(const std::vector<shared<Texture2D>> attachments);

        Vector2 getSize() const;

    private:
        Renderer* renderer;
        FrameBufferHandle handle;
        Vector2 size;
    };

    inline Vector2 FrameBuffer::getSize() const
    {
        return size;
    }
}
