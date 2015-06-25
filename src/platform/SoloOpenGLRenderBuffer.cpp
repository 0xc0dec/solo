#include "SoloOpenGLRenderBuffer.h"

using namespace solo;


OpenGLRenderBuffer::OpenGLRenderBuffer()
{
	glGenRenderbuffers(1, &handle);
}


OpenGLRenderBuffer::~OpenGLRenderBuffer()
{
	if (handle)
		glDeleteRenderbuffers(1, &handle);
}


void OpenGLRenderBuffer::bind()
{
	glBindRenderbuffer(GL_FRAMEBUFFER, handle);
}