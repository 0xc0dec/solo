/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLFrameBuffer.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloDevice.h"
#include "SoloOpenGLTexture.h"

using namespace solo;

static void validateNewAttachments(const vec<sptr<Texture2D>> &attachments)
{
    auto colorAttachmentsCount = 0;
    for (const auto &attachment : attachments)
    {
        const auto isDepthAttachment = attachment->getFormat() == TextureFormat::Depth;
        colorAttachmentsCount += isDepthAttachment ? 0 : 1;
    }
    panicIf(colorAttachmentsCount > GL_MAX_COLOR_ATTACHMENTS, "Too many color attachments");
}

auto OpenGLFrameBuffer::create(const vec<sptr<Texture2D>> &attachments) -> sptr<OpenGLFrameBuffer>
{
    SL_DEBUG_BLOCK(validateNewAttachments(attachments));
    SL_DEBUG_BLOCK(::validateNewAttachments(attachments));

    auto result = sptr<OpenGLFrameBuffer>(new OpenGLFrameBuffer());

    for (const auto &tex : attachments)
    {
        const auto glTex = std::static_pointer_cast<OpenGLTexture2D>(tex);
        if (tex->getFormat() == TextureFormat::Depth)
            result->depthAttachment = glTex;
        else
            result->colorAttachments.push_back(glTex);
    }

    glGenFramebuffers(1, &result->handle);
    panicIf(!result->handle, "Failed to create frame buffer handle");
    
    glBindFramebuffer(GL_FRAMEBUFFER, result->handle);

    for (u32 i = 0; i < result->colorAttachments.size(); i++)
    {
        const auto tex = result->colorAttachments.at(i);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, tex->getHandle(), 0);
    }

    result->dimensions = attachments[0]->getDimensions();

    if (!result->depthAttachment)
    {
        result->depthAttachment = OpenGLTexture2D::createEmpty(
            result->dimensions.x(), result->dimensions.y(), TextureFormat::Depth);
    }

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, result->depthAttachment->getHandle(), 0);

    panicIf(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE, "Frame buffer has invalid state");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return result;
}

OpenGLFrameBuffer::~OpenGLFrameBuffer()
{
    glDeleteFramebuffers(1, &handle);
}

#endif
