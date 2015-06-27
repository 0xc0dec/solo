#include "SoloOpenGLRenderTarget.h"
#include "SoloTexture2D.h"

using namespace solo;


OpenGLRenderTarget::OpenGLRenderTarget():
	depthBufferHandle{0}
{
	glGenFramebuffers(1, &handle);
	if (!handle)
		THROW_FMT(EngineException, "Failed to obtain render target handle.");
}


OpenGLRenderTarget::~OpenGLRenderTarget()
{
	glDeleteFramebuffers(1, &handle);
	if (depthBufferHandle)
		glDeleteRenderbuffers(1, &depthBufferHandle);
}


void OpenGLRenderTarget::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, handle);
}


void OpenGLRenderTarget::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void OpenGLRenderTarget::update()
{
	if (textures.size() > GL_MAX_COLOR_ATTACHMENTS)
		THROW_FMT(EngineException, "Too many color textures for a render target (max allowed: ", GL_MAX_COLOR_ATTACHMENTS, ").");

	bind();

	std::vector<GLenum> colorAttachments;
	for (auto i = 0; i < textures.size(); ++i)
	{
		auto attachment = GL_COLOR_ATTACHMENT0 + i;
		glFramebufferTexture(GL_FRAMEBUFFER, attachment, *reinterpret_cast<GLuint*>(textures[i]->getNativeHandle()), 0);
		colorAttachments.push_back(attachment);
	}
	glDrawBuffers(static_cast<GLsizei>(textures.size()), colorAttachments.data());

	// attach default depth render buffer to have depth testing enabled
	if (textures.size())
	{
		if (!depthBufferHandle)
		{
			glGenRenderbuffers(1, &depthBufferHandle);
			if (!depthBufferHandle)
				THROW_FMT(EngineException, "Could not obtain depth buffer handle.");
		}
		glBindRenderbuffer(GL_RENDERBUFFER, depthBufferHandle);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, textures[0]->getSize().x, textures[0]->getSize().y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferHandle);
	}
	else
		glDeleteRenderbuffers(1, &depthBufferHandle);

	checkStatus();
	unbind();
}


void OpenGLRenderTarget::checkStatus()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		THROW_FMT(EngineException, "Render target has invalid state.");
}
