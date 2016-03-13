#pragma once

#include "SoloComponent.h"
#include "SoloVector4.h"
#include "SoloBitFlags.h"
#include "SoloMatrix.h"
#include "SoloTransform.h"
#include "SoloNode.h"


namespace solo
{
    class Transform;
    class Node;
    class FrameBuffer;
    class Scene;
    class Renderer;
    class Device;

    class Camera final: public ComponentBase<Camera>, protected TransformCallback
    {
    public:
        Camera(Scene* scene, Node node);

        virtual void init() override final;
        virtual void terminate() override final;

        void apply();
        void finish();

        BitFlags& getRenderTags();

        shared<FrameBuffer> getRenderTarget() const;
        void setRenderTarget(shared<FrameBuffer> target);

        Vector4 getClearColor() const;
        void setClearColor(float r, float g, float b, float a);

        Vector4 getViewport() const;
        void setViewport(float left, float top, float width, float height);
        void resetViewport();

        bool isPerspective() const;
        void setPerspective(bool perspective);

        float getNear() const;
        void setNear(float near);

        float getFar() const;
        void setFar(float far);

        float getFOV() const;
        void setFOV(float fov);

        float getWidth() const;
        void setWidth(float width);

        float getHeight() const;
        void setHeight(float height);

        float getAspectRatio() const;
        void setAspectRatio(float ratio);

        const Matrix& getViewMatrix();
        const Matrix& getInverseViewMatrix();
        const Matrix& getProjectionMatrix();
        const Matrix& getViewProjectionMatrix();
        const Matrix& getInverseViewProjectionMatrix();

    protected:
        virtual void onTransformChanged(const Transform* transform) override;

        BitFlags dirtyFlags;
        BitFlags renderTags;

        Device* device;
        Scene* scene;
        Renderer* renderer;

        Transform* transform = nullptr;
        shared<FrameBuffer> renderTarget = nullptr;

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

    inline float Camera::getNear() const
    {
        return nearClip;
    }

    inline float Camera::getFar() const
    {
        return farClip;
    }

    inline float Camera::getFOV() const
    {
        return fov;
    }

    inline float Camera::getWidth() const
    {
        return width;
    }

    inline float Camera::getHeight() const
    {
        return height;
    }

    inline float Camera::getAspectRatio() const
    {
        return aspectRatio;
    }

    inline BitFlags& Camera::getRenderTags()
    {
        return renderTags;
    }

    inline void Camera::setRenderTarget(shared<FrameBuffer> target)
    {
        renderTarget = target;
    }

    inline Vector4 Camera::getClearColor() const
    {
        return clearColor;
    }

    inline shared<FrameBuffer> Camera::getRenderTarget() const
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
