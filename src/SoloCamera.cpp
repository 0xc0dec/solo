#include <SoloEngine.h>
#include "SoloCamera.h"
#include "SoloNode.h"
#include "SoloDevice.h"
#include "platform/SoloOpenGLCamera.h"
#include "SoloLog.h"

using namespace solo;

const unsigned DIRTY_BIT_VIEW = 1;
const unsigned DIRTY_BIT_PROJ = 2;
const unsigned DIRTY_BIT_VIEW_PROJ = 4;
const unsigned DIRTY_BIT_INV_VIEW = 8;
const unsigned DIRTY_BIT_INV_VIEW_PROJ = 16;
const unsigned DIRTY_BIT_VIEWPORT = 32;
const unsigned DIRTY_BIT_CLEAR_COLOR = 64;
const unsigned DIRTY_BIT_ALL = DIRTY_BIT_VIEW | DIRTY_BIT_PROJ | DIRTY_BIT_VIEW_PROJ |
								DIRTY_BIT_INV_VIEW | DIRTY_BIT_INV_VIEW_PROJ |
								DIRTY_BIT_VIEWPORT | DIRTY_BIT_CLEAR_COLOR;


Camera::Camera(Node* node):
	ComponentBase(node),
	ortho(false), viewport(0, 0, 1, 1), clearColor(0, 0, 0, 1),
	fov(60), near(1), far(100), width(1), height(1), aspectRatio(1)
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
	setDirty<DIRTY_BIT_VIEWPORT>();
}


Vector4 Camera::getViewport() const
{
	return viewport;
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
	return near;
}


float Camera::getFar() const
{
	return far;
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
	this->far = far;
	setDirty<DIRTY_BIT_PROJ, DIRTY_BIT_VIEW_PROJ, DIRTY_BIT_INV_VIEW_PROJ>();
}


void Camera::setNear(float near)
{
	this->near = near;
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
			Matrix::createOrthographic(width, height, near, far, &projectionMatrix);
		else
			Matrix::createPerspective(fov, aspectRatio, near, far, &projectionMatrix);
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
	setDirty<DIRTY_BIT_CLEAR_COLOR>();
}


void Camera::update()
{
}


void Camera::render(RenderContext& context)
{
	if (checkAndCleanBit<DIRTY_BIT_VIEWPORT>())
		applyViewportChange();
	if (checkAndCleanBit<DIRTY_BIT_CLEAR_COLOR>())
		applyClearColor();
	clear();
}


shared <Camera> CameraFactory::create(Node* node)
{
	return NEW2(OpenGLCamera, node);
}
