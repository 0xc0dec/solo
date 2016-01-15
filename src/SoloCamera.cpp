#include "SoloNode.h"
#include "SoloDevice.h"
#include "SoloRenderTarget.h"
#include "platform/stub/SoloStubCamera.h"
#include "platform/opengl/SoloOpenGLCamera.h"

using namespace solo;


const unsigned DirtyBitView = 1;
const unsigned DirtyBitProjection = 2;
const unsigned DirtyBitViewProjection = 4;
const unsigned DirtyBitInverseView = 8;
const unsigned DirtyBitInverseViewProjection = 16;
const unsigned DirtyBitAll =
    DirtyBitView |
    DirtyBitProjection |
    DirtyBitViewProjection |
    DirtyBitInverseView |
    DirtyBitInverseViewProjection;


shared<Camera> Camera::create(DeviceMode mode, Scene* scene, Node node)
{
    if (mode == DeviceMode::OpenGL)
        return SL_NEW_SHARED(OpenGLCamera, scene, node);
    return SL_NEW_SHARED(StubCamera, scene, node);
}


Camera::Camera(Scene* scene, Node node):
    ComponentBase(node),
    scene(scene)
{
    renderQueue = KnownRenderQueues::CameraDefault;
    renderTags.setAll();
}


void Camera::init()
{
    transform = node.getComponent<Transform>();
    transform->addCallback(this);
    auto canvasSize = scene->getDevice()->getCanvasSize();
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


Vector4 Camera::getViewport() const
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


const Matrix& Camera::getViewMatrix()
{
    if (dirtyFlags.checkAndRemove(DirtyBitView))
    {
        viewMatrix = transform->getWorldMatrix();
        viewMatrix.invert();
    }
    return viewMatrix;
}


const Matrix& Camera::getInverseViewMatrix()
{
    if (dirtyFlags.checkAndRemove(DirtyBitInverseView))
    {
        inverseViewMatrix = getViewMatrix();
        inverseViewMatrix.invert();
    }
    return inverseViewMatrix;
}


const Matrix& Camera::getProjectionMatrix()
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


const Matrix& Camera::getViewProjectionMatrix()
{
    if (dirtyFlags.checkAndRemove(DirtyBitViewProjection))
        viewProjectionMatrix = getProjectionMatrix() * getViewMatrix();
    return viewProjectionMatrix;
}


const Matrix& Camera::getInverseViewProjectionMatrix()
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
    applyViewport();
    applyClearColor();
    clear();
}


void Camera::finish()
{
    if (renderTarget)
        renderTarget->unbind();
}
