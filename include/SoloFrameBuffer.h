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
        static sptr<FrameBuffer> create();

        ~FrameBuffer();
        SL_NONCOPYABLE(FrameBuffer)

        void bind();
        void unbind();

        void setAttachments(const std::vector<sptr<Texture2D>> attachments);

        Vector2 getSize() const;

    private:
        FrameBuffer();

        Renderer* renderer;
        FrameBufferHandle handle;
        Vector2 size;
    };

    inline Vector2 FrameBuffer::getSize() const
    {
        return size;
    }
}
