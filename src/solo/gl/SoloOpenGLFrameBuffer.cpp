/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLFrameBuffer.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloDevice.h"
#include "SoloOpenGLTexture.h"
#include <algorithm>

using namespace solo;

gl::OpenGLFrameBuffer::OpenGLFrameBuffer()
{
    glGenFramebuffers(1, &handle);
    SL_PANIC_IF(!handle, "Failed to create frame buffer handle");
}

gl::OpenGLFrameBuffer::~OpenGLFrameBuffer()
{
    glDeleteFramebuffers(1, &handle);
}

static void validateNewAttachments(const vec<sptr<solo::Texture2d>> &attachments)
{
    SL_PANIC_IF(attachments.size() > GL_MAX_COLOR_ATTACHMENTS, "Too many attachments");

    auto width = -1, height = -1;
    for (auto i = 0; i < attachments.size(); i++)
    {
        auto size = attachments.at(i)->getDimensions();
        if (width < 0)
        {
            width = size.x;
            height = size.y;
        }
        else
            SL_PANIC_IF(size.x != width || size.y != height, "Attachment sizes do not match");
    }
}

void gl::OpenGLFrameBuffer::setAttachments(const vec<sptr<solo::Texture2d>> &attachments)
{
    SL_PANIC_BLOCK(validateNewAttachments(attachments));

    vec<OpenGLTexture2d*> newAttachments;
    for (const auto &tex : attachments)
        newAttachments.push_back(dynamic_cast<OpenGLTexture2d*>(tex.get()));

    glBindFramebuffer(GL_FRAMEBUFFER, handle);

    if (depthBufferHandle)
    {
        glDeleteRenderbuffers(1, &depthBufferHandle);
        depthBufferHandle = 0;
    }

    const auto newCount = newAttachments.size();
    const auto maxCount = std::max(newCount, static_cast<size_t>(attachmentCount));
    for (auto i = 0; i < maxCount; i++)
    {
        const auto handle = i < newCount ? newAttachments.at(i)->getHandle() : 0;
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, handle, 0);
    }

    dimensions = {0, 0};
    attachmentCount = newCount;

    if (newCount > 0)
    {
        dimensions = attachments[0]->getDimensions();

        // Re-create the depth buffer
        glGenRenderbuffers(1, &depthBufferHandle);
        SL_PANIC_IF(!depthBufferHandle, "Failed to create depth buffer handle");

        glBindRenderbuffer(GL_RENDERBUFFER, depthBufferHandle);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, dimensions.x, dimensions.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferHandle);

        SL_PANIC_IF(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE, "Render target has invalid state");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

#endif
