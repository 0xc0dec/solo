#pragma once

#include "SoloComponent.h"
#include "SoloVector4.h"
#include "SoloMatrix.h"
#include "SoloTransform.h"
#include "SoloNode.h"


namespace solo
{
    class Transform;
    class Node;
    class FrameBuffer;
    class Renderer;
    class Device;

    class Camera final: public ComponentBase<Camera>, protected TransformCallback
    {
    public:
        Camera(Node node);

        virtual void init() override final;
        virtual void terminate() override final;

        void apply();
        void finish();

        auto getRenderTags() -> uint32_t&;

        auto getRenderTarget() const ->sptr<FrameBuffer>;
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

        auto getFOV() const -> float;
        void setFOV(float fov);

        auto getWidth() const -> float;
        void setWidth(float width);

        auto getHeight() const -> float;
        void setHeight(float height);

        auto getAspectRatio() const -> float;
        void setAspectRatio(float ratio);

        auto getViewMatrix() -> const Matrix&;
        auto getInverseViewMatrix() -> const Matrix&;
        auto getProjectionMatrix() -> const Matrix&;
        auto getViewProjectionMatrix() -> const Matrix&;
        auto getInverseViewProjectionMatrix() -> const Matrix&;

    protected:
        virtual void onTransformChanged(const Transform* transform) override;

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
        float fov = 60;
        float nearClip = 1;
        float farClip = 100;
        float width = 1;
        float height = 1;
        float aspectRatio = 1;

        Matrix viewMatrix;
        Matrix projectionMatrix;
        Matrix viewProjectionMatrix;
        Matrix inverseViewMatrix;
        Matrix inverseViewProjectionMatrix;
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

    inline auto Camera::getFOV() const -> float
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
