/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include "SoloComponent.h"
#include "math/SoloVector2.h"
#include "math/SoloVector4.h"
#include "SoloTransform.h"
#include "SoloNode.h"
#include "math/SoloRadians.h"
#include <functional>

namespace solo
{
    class Transform;
    class Node;
    class FrameBuffer;
    class Renderer;
    class Device;
    class Ray;
    struct Radians;

    class Camera: public ComponentBase<Camera>
    {
    public:
        static auto create(const Node &node) -> sptr<Camera>;

        void init() override final;
        void update() override final;

        void renderFrame(const std::function<void()> &render);

        auto windowPointToWorldRay(const Vector2 &pt) const -> Ray;

        auto transform() const -> Transform *
        {
            return transform_;
        }

        auto renderTarget() const -> sptr<FrameBuffer> { return renderTarget_; }
        void setRenderTarget(sptr<FrameBuffer> target)
        {
            renderTarget_ = target;
        }

        auto clearColor() const -> Vector4
        {
            return clearColor_;
        }
        void setClearColor(const Vector4 &color)
        {
            clearColor_ = color;
        }

        bool hasColorClearing() const
        {
            return colorClearing_;
        }
        void setColorClearing(bool enabled)
        {
            this->colorClearing_ = enabled;
        }

        auto viewport() const -> Vector4
        {
            return viewport_;
        }
        void setViewport(const Vector4 &rect)
        {
            viewport_ = rect;
        }

        bool isPerspective() const
        {
            return !ortho_;
        }
        void setPerspective(bool perspective);

        auto zNear() const -> float
        {
            return zNear_;
        }
        void setZNear(float near);

        auto zFar() const -> float
        {
            return zFar_;
        }
        void setZFar(float far);

        auto fieldOfView() const -> Radians
        {
            return fov_;
        }
        void setFieldOfView(const Radians &fov);

        auto orthoSize() const -> Vector2
        {
            return orthoSize_;
        }
        void setOrthoSize(const Vector2 &size);

        auto aspectRatio() const -> float
        {
            return aspectRatio_;
        }

        auto viewMatrix() const -> Matrix;
        auto invViewMatrix() const -> Matrix;
        auto projectionMatrix() const -> Matrix;
        auto viewProjectionMatrix() const -> Matrix;
        auto invViewProjectionMatrix() const -> Matrix;

    protected:
        Device *device_ = nullptr;
        Renderer *renderer_ = nullptr;

        Transform *transform_ = nullptr;
        sptr<FrameBuffer> renderTarget_;

        Vector4 viewport_;
        Vector4 clearColor_{0, 0.5, 0.5, 1};
        bool colorClearing_ = true;
        bool ortho_ = false;
        Vector2 orthoSize_{1, 1};
        Radians fov_;
        float zNear_ = 1;
        float zFar_ = 100;
        float aspectRatio_ = 1;

        mutable u32 lastTransformVersion_ = ~0;
        mutable u32 dirtyFlags_ = ~0;

        mutable Matrix viewMatrix_;
        mutable Matrix projectionMatrix_;
        mutable Matrix viewProjectionMatrix_;
        mutable Matrix invViewMatrix_;
        mutable Matrix invViewProjectionMatrix_;

        explicit Camera(const Node &node);
    };

    template <>
    template <class... Args>
    auto NodeHelper<Camera>::addComponent(Scene *scene, u32 nodeId, Args &&... args) -> Camera *
    {
        const auto body = std::shared_ptr<Camera>(Camera::create(Node(scene, nodeId), std::forward<Args>(args)...));
        scene->addComponent(nodeId, body);
        return body.get();
    }
}
