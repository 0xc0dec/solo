#include "SoloCamera.h"
#include "SoloNode.h"
#include "SoloDevice.h"
#include "SoloFrameBuffer.h"
#include "SoloBitFlags.h"

using namespace solo;


const uint32_t DirtyBitView = 1;
const uint32_t DirtyBitProjection = 2;
const uint32_t DirtyBitViewProjection = 4;
const uint32_t DirtyBitInverseView = 8;
const uint32_t DirtyBitInverseViewProjection = 16;
const uint32_t DirtyBitAll =
    DirtyBitView |
    DirtyBitProjection |
    DirtyBitViewProjection |
    DirtyBitInverseView |
    DirtyBitInverseViewProjection;


Camera::Camera(Node node):
    ComponentBase(node),
    device(Device::get()),
    renderer(Device::get()->getRenderer())
{
    renderQueue = KnownRenderQueues::Camera;
    renderTags.setAll();
}


void Camera::init()
{
    transform = node.getComponent<Transform>();
    transform->addCallback(this);
    auto canvasSize = device->getCanvasSize();
    setAspectRatio(canvasSize.x / canvasSize.y);
    dirtyFlags.add(DirtyBitAll); // arguably
}


void Camera::onTransformChanged(const Transform* transform)
{
    dirtyFlags.add(DirtyBitView | DirtyBitViewProjection | DirtyBitInverseView | DirtyBitInverseViewProjection);
}


void Camera::setViewport(float left, float top, float width, float height)
{
    viewport = Vector4(left, top, width, height);
    viewportSet = true;
}


auto Camera::getViewport() const -> Vector4
{
    if (viewportSet)
        return viewport;
    return Vector4();
}


void Camera::setPerspective(bool perspective)
{
    ortho = !perspective;
    dirtyFlags.add(DirtyBitProjection | DirtyBitViewProjection | DirtyBitInverseViewProjection);
}


void Camera::setFOV(float fov)
{
    this->fov = fov;
    dirtyFlags.add(DirtyBitProjection | DirtyBitViewProjection | DirtyBitInverseViewProjection);
}


void Camera::setWidth(float width)
{
    this->width = width;
    dirtyFlags.add(DirtyBitProjection | DirtyBitViewProjection | DirtyBitInverseViewProjection);
}


void Camera::setHeight(float height)
{
    this->height = height;
    dirtyFlags.add(DirtyBitProjection | DirtyBitViewProjection | DirtyBitInverseViewProjection);
}


void Camera::setAspectRatio(float ratio)
{
    aspectRatio = ratio;
    dirtyFlags.add(DirtyBitProjection | DirtyBitViewProjection | DirtyBitInverseViewProjection);
}


void Camera::setFar(float far)
{
    this->farClip = far;
    dirtyFlags.add(DirtyBitProjection | DirtyBitViewProjection | DirtyBitInverseViewProjection);
}


void Camera::setNear(float near)
{
    this->nearClip = near;
    dirtyFlags.add(DirtyBitProjection | DirtyBitViewProjection | DirtyBitInverseViewProjection);
}


auto Camera::getViewMatrix() -> const Matrix&
{
    if (dirtyFlags.checkAndRemove(DirtyBitView))
    {
        viewMatrix = transform->getWorldMatrix();
        viewMatrix.invert();
    }
    return viewMatrix;
}


auto Camera::getInverseViewMatrix() -> const Matrix&
{
    if (dirtyFlags.checkAndRemove(DirtyBitInverseView))
    {
        inverseViewMatrix = getViewMatrix();
        inverseViewMatrix.invert();
    }
    return inverseViewMatrix;
}


auto Camera::getProjectionMatrix() -> const Matrix&
{
    if (dirtyFlags.checkAndRemove(DirtyBitProjection))
    {
        if (ortho)
            projectionMatrix = Matrix::createOrthographic(width, height, nearClip, farClip);
        else
            projectionMatrix = Matrix::createPerspective(fov, aspectRatio, nearClip, farClip);
    }
    return projectionMatrix;
}


auto Camera::getViewProjectionMatrix() -> const Matrix&
{
    if (dirtyFlags.checkAndRemove(DirtyBitViewProjection))
        viewProjectionMatrix = getProjectionMatrix() * getViewMatrix();
    return viewProjectionMatrix;
}


auto Camera::getInverseViewProjectionMatrix() -> const Matrix&
{
    if (dirtyFlags.checkAndRemove(DirtyBitInverseViewProjection))
    {
        inverseViewProjectionMatrix = getViewProjectionMatrix();
        inverseViewProjectionMatrix.invert();
    }
    return inverseViewProjectionMatrix;
}


void Camera::apply()
{
    if (renderTarget)
        renderTarget->bind();

    if (viewportSet)
    {
        renderer->setViewport(
            static_cast<uint32_t>(viewport.x),
            static_cast<uint32_t>(viewport.y),
            static_cast<uint32_t>(viewport.z),
            static_cast<uint32_t>(viewport.w));
    }
    else
    {
        auto size = device->getCanvasSize();
        renderer->setViewport(0, 0, static_cast<uint32_t>(size.x), static_cast<uint32_t>(size.y));
    }

    renderer->setDepthWrite(true);
    renderer->setDepthTest(true);
    renderer->clear(true, true, clearColor.x, clearColor.y, clearColor.z, clearColor.w);
}


void Camera::finish()
{
    if (renderTarget)
        renderTarget->unbind();
}
