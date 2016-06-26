#pragma once

#include "SoloComponent.h"
#include "SoloVector4.h"
#include "SoloTransformMatrix.h"
#include "SoloTransform.h"
#include "SoloNode.h"


namespace solo
{
    class Transform;
    class Node;
    class FrameBuffer;
    class Renderer;
    class Device;
    struct Radian;

    class Camera final: public ComponentBase<Camera>, protected TransformCallback
    {
    public:
        explicit Camera(const Node& node);

        void init() override final;
        void terminate() override final;

        void apply();
        void finish();

        auto getRenderTags() -> uint32_t&;

        auto getRenderTarget() const -> sptr<FrameBuffer>;
        void setRenderTarget(sptr<FrameBuffer> target);

        auto getClearColor() const -> Vector4;
        void setClearColor(float r, float g, float b, float a);

        auto getViewport() const -> Vector4;
        void setViewport(float left, float top, float width, float height);
        void resetViewport();

        bool isPerspective() const;
        void setPerspective(bool perspective);

        auto getNear() const -> float;
        void setNear(float near);

        auto getFar() const -> float;
        void setFar(float far);

        auto getFOV() const -> Radian;
        void setFOV(const Radian& fov);

        auto getWidth() const -> float;
        void setWidth(float width);

        auto getHeight() const -> float;
        void setHeight(float height);

        auto getAspectRatio() const -> float;
        void setAspectRatio(float ratio);

        auto getViewMatrix() -> const TransformMatrix&;
        auto getInvViewMatrix() -> const TransformMatrix&;
        auto getProjectionMatrix() -> const TransformMatrix&;
        auto getViewProjectionMatrix() -> const TransformMatrix&;
        auto getInvViewProjectionMatrix() -> const TransformMatrix&;

    protected:
        void onTransformChanged(const Transform*, uint32_t) override;

        uint32_t dirtyFlags = ~0;
        uint32_t renderTags = ~0;

        Device* device;
        Renderer* renderer;

        Transform* transform = nullptr;
        sptr<FrameBuffer> renderTarget = nullptr;

        bool ortho = false;

        Vector4 viewport;
        bool viewportSet = false;

        Vector4 clearColor{ 0, 0, 0, 1 };
        Radian fov;
        float nearClip = 1;
        float farClip = 100;
        float width = 1;
        float height = 1;
        float aspectRatio = 1;

        TransformMatrix viewMatrix;
        TransformMatrix projectionMatrix;
        TransformMatrix viewProjectionMatrix;
        TransformMatrix invViewMatrix;
        TransformMatrix invViewProjectionMatrix;
    };

    inline void Camera::setClearColor(float r, float g, float b, float a)
    {
        clearColor = Vector4(r, g, b, a);
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

    inline auto Camera::getRenderTags() -> uint32_t&
    {
        return renderTags;
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

    inline void Camera::resetViewport()
    {
        viewportSet = false;
    }
}
