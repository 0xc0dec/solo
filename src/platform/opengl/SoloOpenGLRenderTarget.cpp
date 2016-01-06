#include "SoloOpenGLRenderTarget.h"
#include "SoloOpenGLTexture2D.h"

using namespace solo;


OpenGLRenderTarget::OpenGLRenderTarget():
    depthBufferHandle{0}
{
    glGenFramebuffers(1, &handle);
    if (!handle)
        SL_THROW_FMT(EngineException, "Failed to obtain render target handle");
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


void OpenGLRenderTarget::setColorAttachment(size_t index, shared<Texture2D> texture)
{
    if (index > GL_MAX_COLOR_ATTACHMENTS)
        SL_THROW_FMT(EngineException, "Given color attachment index is not supported (max allowed: ", GL_MAX_COLOR_ATTACHMENTS, ")");

    if (texture)
    {
        auto textureSize = texture->getSize();

        if (textureSize.x < 1 || textureSize.y < 1)
            SL_THROW(EngineException, "Color attachment must have non-zero size");

        if (!colorAttachments.empty())
        {
            auto existingSize = colorAttachments.begin()->second->getSize();
            if (static_cast<size_t>(textureSize.x) != static_cast<size_t>(existingSize.x) ||
                    static_cast<size_t>(textureSize.y) != static_cast<size_t>(existingSize.y)) // TODO this could be rewritten with an integer vector
                SL_THROW_FMT(EngineException, "The new color attachment size differs from that of already set attachments");
        }

        bind();

        auto textureHandle = static_cast<OpenGLTexture2D *>(texture.get())->getHandle();
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, textureHandle, 0);

        if (colorAttachments.empty()) // this is the first texture set
        {
            glGenRenderbuffers(1, &depthBufferHandle);
            if (!depthBufferHandle)
                SL_THROW_FMT(EngineException, "Could not obtain depth buffer handle");
            glBindRenderbuffer(GL_RENDERBUFFER, depthBufferHandle);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, textureSize.x, textureSize.y);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferHandle);
        }

        checkStatus();
        unbind();

        colorAttachments[index] = texture;
    }
    else // a call to detach texture
    {
        if (colorAttachments.find(index) == colorAttachments.end())
            return;

        bind();

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, 0, 0);

        if (colorAttachments.size() == 1) // this was the last texture still attached
        {
            if (depthBufferHandle)
                glDeleteRenderbuffers(1, &depthBufferHandle);
            depthBufferHandle = 0;
        }

        checkStatus();
        unbind();

        colorAttachments.erase(index);
    }
}


Vector2 OpenGLRenderTarget::getColorAttachmentSize() const
{
    if (colorAttachments.empty())
        return Vector2();
    return colorAttachments.begin()->second->getSize();
}


size_t OpenGLRenderTarget::getColorAttachmentCount() const
{
    return colorAttachments.size();
}


shared<Texture2D> OpenGLRenderTarget::getColorAttachment(size_t index) const
{
    auto where = colorAttachments.find(index);
    if (where == colorAttachments.end())
        SL_THROW_FMT(EngineException, "Invalid color attachment index ", index);
    return where->second;
}


void OpenGLRenderTarget::checkStatus()
{
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        SL_THROW_FMT(EngineException, "Render target has invalid state");
}
