#pragma once

#include "SoloRenderTarget.h"
#include "SoloVector2.h"
#include <unordered_map>
#include <GL/glew.h>

namespace solo
{
    class OpenGLRenderTarget: public RenderTarget
    {
    public:
        virtual ~OpenGLRenderTarget();

        virtual void bind() override;
        virtual void unbind() override;

        virtual void setColorAttachment(size_t index, shared<Texture2D> texture) override;
        virtual Vector2 getColorAttachmentSize() const override;
        virtual size_t getColorAttachmentCount() const override;
        virtual shared<Texture2D> getColorAttachment(size_t index) const override;

    private:
        friend class RenderTarget;

        OpenGLRenderTarget();

        static void checkStatus();

        GLuint handle = 0;
        GLuint depthBufferHandle = 0;
        std::unordered_map<size_t, shared<Texture2D>> colorAttachments;
        Vector2 colorAttachmentSize;
        bool anyColorAttachments = false;
    };
}
