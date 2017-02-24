/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include "SoloComponent.h"
#include "SoloVector4.h"
#include "SoloTransformMatrix.h"
#include "SoloTransform.h"
#include "SoloNode.h"
#include "SoloRadian.h"
#include <functional>


namespace solo
{
    class Transform;
    class Node;
    class FrameBuffer;
    class Renderer;
    class Device;
    struct RenderContext;
    struct Radian;

    class Camera: public ComponentBase<Camera>, protected TransformCallback
    {
    public:
        static auto create(const Node &node) -> sptr<Camera>;

        void init() override final;
        void terminate() override final;

        // TODO perhaps specify render target to add more "functional" flavor
        void renderFrame(std::function<void(const RenderContext&)> render) const;

        auto getTransform() const -> Transform*;

        auto getRenderTarget() const -> sptr<FrameBuffer>;
        void setRenderTarget(sptr<FrameBuffer> target);

        auto getClearColor() const -> Vector4;
        void setClearColor(const Vector4 &color);

        bool isClearColorEnabled() const;
        void setClearColorEnabled(bool clear);

        bool isClearDepthEnabled() const;
        void setClearDepthEnabled(bool clear);

        auto getViewport() const -> Vector4;
        void setViewport(const Vector4 &rect);

        bool isPerspective() const;
        void setPerspective(bool perspective);

        auto getNear() const -> float;
        void setNear(float near);

        auto getFar() const -> float;
        void setFar(float far);

        auto getFOV() const -> Radian;
        void setFOV(const Radian &fov);

        auto getWidth() const -> float;
        void setWidth(float width);

        auto getHeight() const -> float;
        void setHeight(float height);

        auto getAspectRatio() const -> float;
        void setAspectRatio(float ratio);

        auto getViewMatrix() const -> const TransformMatrix;
        auto getInvViewMatrix() const -> const TransformMatrix;
        auto getProjectionMatrix() const -> const TransformMatrix;
        auto getViewProjectionMatrix() const -> const TransformMatrix;
        auto getInvViewProjectionMatrix() const -> const TransformMatrix;

    protected:
        Device *device = nullptr;

        Transform *transform = nullptr;
        sptr<FrameBuffer> renderTarget = nullptr;

        struct
        {
            bool color = true;
            bool depth = true;
        } clearFlags;

        Vector4 viewport;
        Vector4 clearColor = {0, 0.5, 0.5, 1};
        bool ortho = false;
        Radian fov;
        float nearClip = 1;
        float farClip = 100;
        float width = 1;
        float height = 1;
        float aspectRatio = 1;

        mutable uint32_t transformDirtyFlags = ~0;

        mutable TransformMatrix viewMatrix;
        mutable TransformMatrix projectionMatrix;
        mutable TransformMatrix viewProjectionMatrix;
        mutable TransformMatrix invViewMatrix;
        mutable TransformMatrix invViewProjectionMatrix;

        explicit Camera(const Node &node);
        
        virtual void beginFrame() const = 0;
        virtual void endFrame() const = 0;
        void onTransformChanged(const Transform *, uint32_t) override;
    };

    inline void Camera::setClearColor(const Vector4 &color)
    {
        clearColor = color;
    }

    inline bool Camera::isPerspective() const
    {
        return !ortho;
    }

    inline auto Camera::getNear() const -> float
    {
        return nearClip;
    }

    inline auto Camera::getFar() const -> float
    {
        return farClip;
    }

    inline auto Camera::getFOV() const -> Radian
    {
        return fov;
    }

    inline auto Camera::getWidth() const -> float
    {
        return width;
    }

    inline auto Camera::getHeight() const -> float
    {
        return height;
    }

    inline auto Camera::getAspectRatio() const -> float
    {
        return aspectRatio;
    }

    inline void Camera::setRenderTarget(sptr<FrameBuffer> target)
    {
        renderTarget = target;
    }

    inline auto Camera::getClearColor() const -> Vector4
    {
        return clearColor;
    }

    inline auto Camera::getRenderTarget() const -> sptr<FrameBuffer>
    {
        return renderTarget;
    }

    inline void Camera::terminate()
    {
        transform->removeCallback(this);
    }

    inline auto Camera::getViewport() const -> Vector4
    {
        return viewport;
    }

    inline void Camera::setViewport(const Vector4 &rect)
    {
        viewport = rect;
    }

    inline auto Camera::getTransform() const -> Transform*
    {
        return transform;
    }

    inline bool Camera::isClearColorEnabled() const
    {
        return clearFlags.color;
    }

    inline void Camera::setClearColorEnabled(bool clear)
    {
        this->clearFlags.color = clear;
    }

    inline bool Camera::isClearDepthEnabled() const
    {
        return clearFlags.depth;
    }

    inline void Camera::setClearDepthEnabled(bool clear)
    {
        this->clearFlags.depth = clear;
    }

    template <>
    template <class... Args>
    auto NodeHelper<Camera>::addComponent(Scene *scene, uint32_t nodeId, Args &&... args) -> Camera *
    {
        auto body = std::shared_ptr<Camera>(Camera::create(Node(scene, nodeId), std::forward<Args>(args)...));
        scene->addComponent(nodeId, body);
        return body.get();
    }
}
