/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLFrameBuffer.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloDevice.h"
#include "SoloOpenGLTexture.h"
#include "SoloTextureData.h"

using namespace solo;

static void validateNewAttachments(const vec<sptr<Texture2D>> &attachments)
{
	// TODO validate depth attachment?
    panicIf(attachments.size() > GL_MAX_COLOR_ATTACHMENTS, "Too many attachments");
    panicIf(attachments.empty(), "Frame buffer must have at least one attachment"); // TODO is it a temp check?

    auto width = -1, height = -1;
    for (const auto &attachment : attachments)
    {
	    const auto size = attachment->getDimensions();
        if (width < 0)
        {
            width = size.x();
            height = size.y();
        }
        else
            panicIf(size.x() != width || size.y() != height, "Attachment sizes do not match");
    }
}

auto OpenGLFrameBuffer::create(const vec<sptr<Texture2D>> &attachments) -> sptr<OpenGLFrameBuffer>
{
	SL_DEBUG_BLOCK(validateNewAttachments(attachments));

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

	for (auto i = 0; i < result->colorAttachments.size(); i++)
	{
		const auto tex = result->colorAttachments.at(i);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, tex->getHandle(), 0);
	}

	if (!result->depthAttachment)
	{
		auto dimensions = attachments[0]->getDimensions();
		auto data = Texture2DData::createFromMemory(dimensions.x(), dimensions.y(), TextureFormat::Depth, vec<u8>{});
		result->depthAttachment = OpenGLTexture2D::createFromData(data.get(), false);
	}

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, result->depthAttachment->getHandle(), 0);

//    glGenRenderbuffers(1, &result->depthBufferHandle);
//    panicIf(!result->depthBufferHandle, "Failed to create depth buffer handle");
//
//	if (!hasDepthAttachment) // generate depth attachment automatically if none passed
//	{
//		auto dimensions = attachments[0]->getDimensions();
//		glBindRenderbuffer(GL_RENDERBUFFER, result->depthBufferHandle);
//		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, dimensions.x(), dimensions.y());
//		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, result->depthBufferHandle);
//	}
    
	panicIf(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE, "Frame buffer has invalid state");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return result;
}

OpenGLFrameBuffer::~OpenGLFrameBuffer()
{
    glDeleteFramebuffers(1, &handle);
}

#endif
