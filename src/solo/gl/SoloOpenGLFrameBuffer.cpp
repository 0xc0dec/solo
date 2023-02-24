/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloOpenGLFrameBuffer.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloDevice.h"
#include "SoloOpenGLTexture.h"

using namespace solo;

static void validateNewAttachments(const vec<sptr<Texture2D>> &attachments) {
    auto colorAttachmentsCount = 0;
    for (const auto &attachment : attachments) {
        const auto isDepthAttachment = attachment->format() == TextureFormat::Depth24;
        colorAttachmentsCount += isDepthAttachment ? 0 : 1;
    }

    panicIf(colorAttachmentsCount > GL_MAX_COLOR_ATTACHMENTS, "Too many color attachments");
}

auto OpenGLFrameBuffer::fromAttachments(const vec<sptr<Texture2D>> &attachments) -> sptr<OpenGLFrameBuffer> {
    asrt([&attachments]() {
        validateNewAttachments(attachments);
    });
    asrt([&attachments]() {
        ::validateNewAttachments(attachments);
    });

    auto result = sptr<OpenGLFrameBuffer>(new OpenGLFrameBuffer());

    for (const auto &tex : attachments) {
        const auto glTex = std::static_pointer_cast<OpenGLTexture2D>(tex);
        if (tex->format() == TextureFormat::Depth24)
            result->depthAttachment_ = glTex;
        else
            result->colorAttachments_.push_back(glTex);
    }

    glGenFramebuffers(1, &result->handle_);
    panicIf(!result->handle_, "Unable to create frame buffer handle");

    glBindFramebuffer(GL_FRAMEBUFFER, result->handle_);

    vec<GLenum> drawBuffers;
    for (u32 i = 0; i < result->colorAttachments_.size(); i++) {
        const auto tex = result->colorAttachments_.at(i);
        const auto attachment = GL_COLOR_ATTACHMENT0 + i;
        glFramebufferTexture(GL_FRAMEBUFFER, attachment, tex->handle(), 0);
        drawBuffers.push_back(attachment);
    }

    glDrawBuffers(drawBuffers.size(), drawBuffers.data());

    result->dimensions_ = attachments[0]->dimensions();

    if (!result->depthAttachment_) {
        result->depthAttachment_ = OpenGLTexture2D::empty(
            result->dimensions_.x(), result->dimensions_.y(), TextureFormat::Depth24);
    }

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, result->depthAttachment_->handle(), 0);

    panicIf(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE, "Frame buffer has invalid state");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return result;
}

OpenGLFrameBuffer::~OpenGLFrameBuffer() {
    glDeleteFramebuffers(1, &handle_);
}

#endif
