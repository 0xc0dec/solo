/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLFrameBuffer.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloDevice.h"
#include "SoloRectTexture.h"
#include "SoloOpenGLRectTexture.h"
#include <algorithm>

using namespace solo;

gl::FrameBuffer::FrameBuffer()
{
    glGenFramebuffers(1, &handle);
    SL_PANIC_IF(!handle, "Failed to create frame buffer handle");
}

gl::FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &handle);
}

static void validateNewAttachments(const std::vector<sptr<solo::RectTexture>> &attachments)
{
    SL_PANIC_IF(attachments.size() > GL_MAX_COLOR_ATTACHMENTS, "Too many attachments");

    auto width = -1, height = -1;
    for (auto i = 0; i < attachments.size(); i++)
    {
        auto size = attachments.at(i)->getSize();
        if (width < 0)
        {
            width = size.x;
            height = size.y;
        }
        else
            SL_PANIC_IF(size.x != width || size.y != height, "Attachment sizes do not match");
    }
}

void gl::FrameBuffer::setAttachments(const std::vector<sptr<solo::RectTexture>> &attachments)
{
    SL_PANIC_BLOCK(validateNewAttachments(attachments));

    std::vector<RectTexture*> newAttachments;
    for (const auto &tex : attachments)
        newAttachments.push_back(dynamic_cast<RectTexture*>(tex.get()));

    glBindFramebuffer(GL_FRAMEBUFFER, handle);

    if (depthBufferHandle)
    {
        glDeleteRenderbuffers(1, &depthBufferHandle);
        depthBufferHandle = 0;
    }

    auto newCount = newAttachments.size();
    auto maxCount = std::max(newCount, static_cast<size_t>(attachmentCount));
    for (auto i = 0; i < maxCount; i++)
    {
        auto handle = i < newCount ? newAttachments.at(i)->getHandle() : 0;
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, handle, 0);
    }

    size = {0, 0};
    attachmentCount = newCount;

    if (newCount > 0)
    {
        size = attachments[0]->getSize();

        // Re-create the depth buffer
        glGenRenderbuffers(1, &depthBufferHandle);
        SL_PANIC_IF(!depthBufferHandle, "Failed to create depth buffer handle");

        glBindRenderbuffer(GL_RENDERBUFFER, depthBufferHandle);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferHandle);

        SL_PANIC_IF(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE, "Render target has invalid state");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

#endif