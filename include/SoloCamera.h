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
        static auto create(const Node& node) -> sptr<Camera>;

        void init() override final;
        void terminate() override final;

        void apply() const;
        void finish() const;

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
        explicit Camera(const Node& node);

        void onTransformChanged(const Transform*, uint32_t) override;

        uint32_t dirtyFlags = ~0;
        uint32_t renderTags = ~0;

        Device* device;
        Renderer* renderer;

        Transform* transform = nullptr;
        sptr<FrameBuffer> renderTarget = nullptr;

        Vector4 viewport;
        bool ortho = false;
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

    template <>
    template <class... Args>
    auto NodeHelper<Camera>::addComponent(Scene* scene, uint32_t nodeId, Args&&... args) -> Camera*
    {
        auto body = std::shared_ptr<Camera>(Camera::create(Node(scene, nodeId), std::forward<Args>(args)...));
        scene->addComponent(nodeId, body);
        return body.get();
    }

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
