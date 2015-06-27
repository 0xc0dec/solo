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
	deleteDepthBuffer();
}


void OpenGLRenderTarget::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, handle);
}


void OpenGLRenderTarget::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void OpenGLRenderTarget::applyTextures(const std::vector<shared<Texture2D>>& textures)
{
	if (textures.size() > GL_MAX_COLOR_ATTACHMENTS)
		THROW_FMT(EngineException, "Too many color textures for a render target (max allowed: ", GL_MAX_COLOR_ATTACHMENTS, ").");

	auto commonWidth = static_cast<int>(textures[0]->getSize().x);
	auto commonHeight = static_cast<int>(textures[0]->getSize().y);
	for (auto tex: textures)
	{
		auto size = tex->getSize();
		if (static_cast<int>(size.x) != commonWidth || static_cast<int>(size.y) != commonHeight)
			THROW_FMT(EngineException, "All color textures for a render target must have the same size.");
	}

	bind();
	deleteDepthBuffer();

	std::vector<GLenum> colorAttachmentBuf;
	for (auto i = 0; i < textures.size(); ++i)
	{
		auto attachment = GL_COLOR_ATTACHMENT0 + i;
		glFramebufferTexture(GL_FRAMEBUFFER, attachment, *reinterpret_cast<GLuint*>(textures[i]->getNativeHandle()), 0);
		colorAttachmentBuf.push_back(attachment);
	}
	glDrawBuffers(static_cast<GLsizei>(textures.size()), colorAttachmentBuf.data());

	// attach default depth render buffer to have depth testing enabled
	glGenRenderbuffers(1, &depthBufferHandle);
	if (!depthBufferHandle)
		THROW_FMT(EngineException, "Could not obtain depth buffer handle.");
	glBindRenderbuffer(GL_RENDERBUFFER, depthBufferHandle);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, commonWidth, commonHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferHandle);

	checkStatus();
	unbind();
}


void OpenGLRenderTarget::checkStatus()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		THROW_FMT(EngineException, "Render target has invalid state.");
}


void OpenGLRenderTarget::deleteDepthBuffer()
{
	if (depthBufferHandle)
		glDeleteRenderbuffers(1, &depthBufferHandle);
	depthBufferHandle = 0;
}