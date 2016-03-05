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

        virtual void setColorAttachment(int index, shared<Texture2D> texture) override;
        virtual Vector2 getColorAttachmentSize() const override;
        virtual int getColorAttachmentCount() const override;
        virtual shared<Texture2D> getColorAttachment(int index) const override;

    private:
        friend class RenderTarget;

        OpenGLRenderTarget();

        static void checkStatus();

        GLuint handle = 0;
        GLuint depthBufferHandle = 0;
        bool anyColorAttachments = false;
        Vector2 colorAttachmentSize;
        std::unordered_map<int, shared<Texture2D>> colorAttachments;
    };
}
