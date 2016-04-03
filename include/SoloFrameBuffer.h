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
        static auto create() -> sptr<FrameBuffer>;

        ~FrameBuffer();
        SL_NONCOPYABLE(FrameBuffer)

        void bind();
        void unbind();

        void setAttachments(const std::vector<sptr<Texture2D>>& attachments);

        auto getSize() const -> Vector2;

    private:
        FrameBuffer();

        Renderer* renderer;
        FrameBufferHandle handle;
        Vector2 size;
    };

    inline auto FrameBuffer::getSize() const -> Vector2
    {
        return size;
    }
}
