/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloComponent.h"
#include "SoloVector2.h"
#include "SoloVector4.h"
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
    struct Radian;

    class Camera: public ComponentBase<Camera>, protected TransformCallback
    {
    public:
        static auto create(const Node &node) -> sptr<Camera>;

        void init() override final;
        void terminate() override final;

        void renderFrame(std::function<void()> render) const;

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

        auto getOrthoSize() const -> Vector2;
        void setOrthoSize(const Vector2 &size);

        auto getAspectRatio() const -> float;
        void setAspectRatio(float ratio);

        auto getViewMatrix() const -> const Matrix;
        auto getInvViewMatrix() const -> const Matrix;
        auto getProjectionMatrix() const -> const Matrix;
        auto getViewProjectionMatrix() const -> const Matrix;
        auto getInvViewProjectionMatrix() const -> const Matrix;

    protected:
        Device *device = nullptr;
        Renderer *renderer = nullptr;

        Transform *transform = nullptr;
        sptr<FrameBuffer> renderTarget = nullptr;

        struct
        {
            bool color = true;
            bool depth = true;
        } clearFlags;

        Vector4 viewport;
        Vector4 clearColor{0, 0.5, 0.5, 1};
        bool ortho = false;
        Vector2 orthoSize{1, 1};
        Radian fov;
        float nearClip = 1;
        float farClip = 100;
        float aspectRatio = 1;

        mutable uint32_t dirtyFlags = ~0;

        mutable Matrix viewMatrix;
        mutable Matrix projectionMatrix;
        mutable Matrix viewProjectionMatrix;
        mutable Matrix invViewMatrix;
        mutable Matrix invViewProjectionMatrix;

        explicit Camera(const Node &node);

        void onTransformChanged(const Transform *) override;
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

    inline auto Camera::getOrthoSize() const -> Vector2
    {
        return orthoSize;
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
