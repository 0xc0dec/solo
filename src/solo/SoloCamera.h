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
    class Ray;
    struct Radian;

    class Camera: public ComponentBase<Camera>, protected TransformCallback
    {
    public:
        static auto create(const Node &node) -> sptr<Camera>;

        void init() override final;
        void terminate() override final { transform->removeCallback(this); }

        void renderFrame(std::function<void()> render);

        auto canvasPointToWorldRay(const Vector2 &canvasPoint) -> Ray;

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

        auto getFOV() const -> Radian { return fov; }
        void setFOV(const Radian &fov);

        auto getOrthoSize() const -> Vector2 { return orthoSize; }
        void setOrthoSize(const Vector2 &size);

        auto getAspectRatio() const -> float { return aspectRatio; }

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

        Vector4 viewport;
        Vector4 clearColor{0, 0.5, 0.5, 1};
        bool colorClearing = true;
        bool ortho = false;
        Vector2 orthoSize{1, 1};
        Radian fov;
        float zNear = 1;
        float zFar = 100;
        float aspectRatio = 1;

        mutable u32 dirtyFlags = ~0;

        mutable Matrix viewMatrix;
        mutable Matrix projectionMatrix;
        mutable Matrix viewProjectionMatrix;
        mutable Matrix invViewMatrix;
        mutable Matrix invViewProjectionMatrix;

        explicit Camera(const Node &node);

        void onTransformChanged(const Transform *) override;
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
