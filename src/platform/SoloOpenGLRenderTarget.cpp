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


void OpenGLRenderTarget::applyTexture()
{
	static GLenum buffers = GL_COLOR_ATTACHMENT0;

	bind();
	deleteDepthBuffer();

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, *reinterpret_cast<GLuint*>(texture->getNativeHandle()), 0);
	glDrawBuffers(1, &buffers);

	// attach default depth render buffer to have depth testing enabled
	glGenRenderbuffers(1, &depthBufferHandle);
	if (!depthBufferHandle)
		THROW_FMT(EngineException, "Could not obtain depth buffer handle.");
	glBindRenderbuffer(GL_RENDERBUFFER, depthBufferHandle);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, texture->getSize().x, texture->getSize().y);
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