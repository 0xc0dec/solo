#include <SoloEngine.h>
#include "SoloCamera.h"
#include "SoloNode.h"
#include "SoloDevice.h"
#include "SoloRenderTarget.h"
#include "platform/SoloOpenGLCamera.h"

using namespace solo;


const unsigned DIRTY_BIT_VIEW = 1;
const unsigned DIRTY_BIT_PROJ = 2;
const unsigned DIRTY_BIT_VIEW_PROJ = 4;
const unsigned DIRTY_BIT_INV_VIEW = 8;
const unsigned DIRTY_BIT_INV_VIEW_PROJ = 16;
const unsigned DIRTY_BIT_ALL = DIRTY_BIT_VIEW | DIRTY_BIT_PROJ | DIRTY_BIT_VIEW_PROJ | DIRTY_BIT_INV_VIEW | DIRTY_BIT_INV_VIEW_PROJ;


Camera::Camera(Node* node): ComponentBase(node)
{
	transform = node->getComponent<Transform>();
	transform->addCallback(this);
	auto canvasSize = Engine::get()->getDevice()->getCanvasSize();
	setAspectRatio(canvasSize.x / canvasSize.y);
	setDirty<DIRTY_BIT_ALL>(); // arguably
}


Camera::~Camera()
{
	transform->removeCallback(this);
}


void Camera::onTransformChanged(const Transform* transform)
{
	setDirty<DIRTY_BIT_VIEW, DIRTY_BIT_VIEW_PROJ, DIRTY_BIT_INV_VIEW, DIRTY_BIT_INV_VIEW_PROJ>();
}


void Camera::setViewport(float left, float top, float width, float height)
{
	viewport.set(left, top, width, height);
	viewportSet = true;
}


void Camera::resetViewport()
{
	viewportSet = false;
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
	setDirty<DIRTY_BIT_PROJ, DIRTY_BIT_VIEW_PROJ, DIRTY_BIT_INV_VIEW_PROJ>();
}


bool Camera::isPerspective() const
{
	return !ortho;
}


float Camera::getNear() const
{
	return nearClip;
}


float Camera::getFar() const
{
	return farClip;
}


float Camera::getFOV() const
{
	return fov;
}


float Camera::getWidth() const
{
	return width;
}


float Camera::getHeight() const
{
	return height;
}


float Camera::getAspectRatio() const
{
	return aspectRatio;
}


void Camera::setFOV(float fov)
{
	this->fov = fov;
	setDirty<DIRTY_BIT_PROJ, DIRTY_BIT_VIEW_PROJ, DIRTY_BIT_INV_VIEW_PROJ>();
}


void Camera::setWidth(float width)
{
	this->width = width;
	setDirty<DIRTY_BIT_PROJ, DIRTY_BIT_VIEW_PROJ, DIRTY_BIT_INV_VIEW_PROJ>();
}


void Camera::setHeight(float height)
{
	this->height = height;
	setDirty<DIRTY_BIT_PROJ, DIRTY_BIT_VIEW_PROJ, DIRTY_BIT_INV_VIEW_PROJ>();
}


void Camera::setAspectRatio(float ratio)
{
	aspectRatio = ratio;
	setDirty<DIRTY_BIT_PROJ, DIRTY_BIT_VIEW_PROJ, DIRTY_BIT_INV_VIEW_PROJ>();
}


void Camera::setFar(float far)
{
	this->farClip = far;
	setDirty<DIRTY_BIT_PROJ, DIRTY_BIT_VIEW_PROJ, DIRTY_BIT_INV_VIEW_PROJ>();
}


void Camera::setNear(float near)
{
	this->nearClip = near;
	setDirty<DIRTY_BIT_PROJ, DIRTY_BIT_VIEW_PROJ, DIRTY_BIT_INV_VIEW_PROJ>();
}


const Matrix& Camera::getViewMatrix()
{
	if (checkAndCleanBit<DIRTY_BIT_VIEW>())
		transform->getWorldMatrix().invert(&viewMatrix);
	return viewMatrix;
}


const Matrix& Camera::getInverseViewMatrix()
{
	if (checkAndCleanBit<DIRTY_BIT_INV_VIEW>())
		getViewMatrix().invert(&inverseViewMatrix);
	return inverseViewMatrix;
}


const Matrix& Camera::getProjectionMatrix()
{
	if (checkAndCleanBit<DIRTY_BIT_PROJ>())
	{
		if (ortho)
			Matrix::createOrthographic(width, height, nearClip, farClip, &projectionMatrix);
		else
			Matrix::createPerspective(fov, aspectRatio, nearClip, farClip, &projectionMatrix);
	}
	return projectionMatrix;
}


const Matrix& Camera::getViewProjectionMatrix()
{
	if (checkAndCleanBit<DIRTY_BIT_VIEW_PROJ>())
		Matrix::multiply(getProjectionMatrix(), getViewMatrix(), &viewProjectionMatrix);
	return viewProjectionMatrix;
}


const Matrix& Camera::getInverseViewProjectionMatrix()
{
	if (checkAndCleanBit<DIRTY_BIT_INV_VIEW_PROJ>())
		getViewProjectionMatrix().invert(&inverseViewProjectionMatrix);
	return inverseViewProjectionMatrix;
}


void Camera::setClearColor(float r, float g, float b, float a)
{
	clearColor.set(r, g, b, a);
}


void Camera::update()
{
}


void Camera::render(RenderContext& context)
{
	if (renderTarget)
		renderTarget->bind();
	applyViewport();
	applyClearColor();
	clear();
}


void Camera::postRender()
{
	if (renderTarget)
		renderTarget->unbind();
}


void Camera::setRenderTarget(shared<RenderTarget> target)
{
	renderTarget = target;
}


shared<RenderTarget> Camera::getRenderTarget() const
{
	return renderTarget;
}


shared<Camera> CameraFactory::create(Node* node)
{
	return NEW2(OpenGLCamera, node);
}
