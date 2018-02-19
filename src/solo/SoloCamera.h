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
#include "SoloRadians.h"
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

        auto getTransform() const -> Transform* { return transform; }

        auto getRenderTarget() const -> sptr<FrameBuffer> { return renderTarget; }
        void setRenderTarget(sptr<FrameBuffer> target) { renderTarget = target; }

        auto getClearColor() const -> Vector4 { return clearColor; }
        void setClearColor(const Vector4 &color) { clearColor = color; }

        bool hasColorClearing() const { return colorClearing; }
        void setColorClearing(bool enabled) { this->colorClearing = enabled; }

        auto getViewport() const -> Vector4 { return viewport; }
        void setViewport(const Vector4 &rect) { viewport = rect; }

        bool isPerspective() const { return !ortho; }
        void setPerspective(bool perspective);

        auto getZNear() const -> float { return zNear; }
        void setZNear(float near);

        auto getZFar() const -> float { return zFar; }
        void setZFar(float far);

        auto getFOV() const -> Radians { return fov; }
        void setFOV(const Radians &fov);

        auto getOrthoSize() const -> Vector2 { return orthoSize; }
        void setOrthoSize(const Vector2 &size);

        auto getAspectRatio() const -> float { return aspectRatio; }

        auto getViewMatrix() const -> Matrix;
        auto getInvViewMatrix() const -> Matrix;
        auto getProjectionMatrix() const -> Matrix;
        auto getViewProjectionMatrix() const -> Matrix;
        auto getInvViewProjectionMatrix() const -> Matrix;

    protected:
        Device *device = nullptr;
        Renderer *renderer = nullptr;

        Transform *transform = nullptr;
        sptr<FrameBuffer> renderTarget = nullptr;

        Vector4 viewport;
        Vector4 clearColor{0, 0.5, 0.5, 1};
        bool colorClearing = true;
        bool ortho = false;
        Vector2 orthoSize{1, 1};
        Radians fov;
        float zNear = 1;
        float zFar = 100;
        float aspectRatio = 1;

        mutable u32 lastTransformVersion = ~0;
        mutable u32 dirtyFlags = ~0;

        mutable Matrix viewMatrix;
        mutable Matrix projectionMatrix;
        mutable Matrix viewProjectionMatrix;
        mutable Matrix invViewMatrix;
        mutable Matrix invViewProjectionMatrix;

        explicit Camera(const Node &node);
    };

    template <>
    template <class... Args>
    auto NodeHelper<Camera>::addComponent(Scene *scene, u32 nodeId, Args &&... args) -> Camera*
    {
        auto body = std::shared_ptr<Camera>(Camera::create(Node(scene, nodeId), std::forward<Args>(args)...));
        scene->addComponent(nodeId, body);
        return body.get();
    }
}
