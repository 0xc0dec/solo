#include "SoloCamera.h"
#include "SoloTransform.h"
#include "SoloEngine.h"
#include "SoloScene.h"
#include "platform/SoloOpenGLCamera.h"

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


Camera::Camera(size_t node):
	ComponentBase(node),
	_ortho(false), _viewport(0, 0, 1, 1), _clearColor(0, 0, 0, 1),
	_fov(60), _near(1), _far(100), _width(1), _height(1), _aspectRatio(16.f/9.f)
{
	_transform = Engine::get()->getScene()->getComponent<Transform>(node);
	setDirty<DIRTY_BIT_ALL>(); // arguably
}


ptr<Camera> Camera::create(size_t node)
{
	return OpenGLCamera::create(node);
}


void Camera::setViewport(float left, float top, float width, float height)
{
	_viewport.set(left, top, width, height);
	setDirty<DIRTY_BIT_VIEWPORT>();
}


Vector4 Camera::getViewport() const
{
	return _viewport;
}


void Camera::setPerspective(bool perspective)
{
	_ortho = !perspective;
	setDirty<DIRTY_BIT_PROJ, DIRTY_BIT_VIEW_PROJ, DIRTY_BIT_INV_VIEW_PROJ>();
}


bool Camera::isPerspective() const
{
	return !_ortho;
}


float Camera::getNear() const
{
	return _near;
}


float Camera::getFar() const
{
	return _far;
}


float Camera::getFOV() const
{
	return _fov;
}


float Camera::getWidth() const
{
	return _width;
}


float Camera::getHeight() const
{
	return _height;
}


float Camera::getAspectRatio() const
{
	return _aspectRatio;
}


void Camera::setFOV(float fov)
{
	_fov = fov;
	setDirty<DIRTY_BIT_PROJ, DIRTY_BIT_VIEW_PROJ, DIRTY_BIT_INV_VIEW_PROJ>();
}


void Camera::setWidth(float width)
{
	_width = width;
	setDirty<DIRTY_BIT_PROJ, DIRTY_BIT_VIEW_PROJ, DIRTY_BIT_INV_VIEW_PROJ>();
}


void Camera::setHeight(float height)
{
	_height = height;
	setDirty<DIRTY_BIT_PROJ, DIRTY_BIT_VIEW_PROJ, DIRTY_BIT_INV_VIEW_PROJ>();
}


void Camera::setAspectRatio(float ratio)
{
	_aspectRatio = ratio;
	setDirty<DIRTY_BIT_PROJ, DIRTY_BIT_VIEW_PROJ, DIRTY_BIT_INV_VIEW_PROJ>();
}


void Camera::setFar(float far)
{
	_far = far;
	setDirty<DIRTY_BIT_PROJ, DIRTY_BIT_VIEW_PROJ, DIRTY_BIT_INV_VIEW_PROJ>();
}


void Camera::setNear(float near)
{
	_near = near;
	setDirty<DIRTY_BIT_PROJ, DIRTY_BIT_VIEW_PROJ, DIRTY_BIT_INV_VIEW_PROJ>();
}


const Matrix& Camera::getViewMatrix()
{
	if (checkAndCleanBit<DIRTY_BIT_VIEW>())
		_transform->getWorldMatrix().invert(&_viewMatrix);
	return _viewMatrix;
}


const Matrix& Camera::getInverseViewMatrix()
{
	if (checkAndCleanBit<DIRTY_BIT_INV_VIEW>())
		getViewMatrix().invert(&_inverseViewMatrix);
	return _inverseViewMatrix;
}


const Matrix& Camera::getProjectionMatrix()
{
	if (checkAndCleanBit<DIRTY_BIT_PROJ>())
	{
		if (_ortho)
			Matrix::createOrthographic(_width, _height, _near, _far, &_projectionMatrix);
		else
			Matrix::createPerspective(_fov, _aspectRatio, _near, _far, &_projectionMatrix);
	}
	return _projectionMatrix;
}


const Matrix& Camera::getViewProjectionMatrix()
{
	if (checkAndCleanBit<DIRTY_BIT_VIEW_PROJ>())
		Matrix::multiply(getViewMatrix(), getProjectionMatrix(), &_viewProjectionMatrix);
	return _viewProjectionMatrix;
}


const Matrix& Camera::getInverseViewProjectionMatrix()
{
	if (checkAndCleanBit<DIRTY_BIT_INV_VIEW_PROJ>())
		getViewProjectionMatrix().invert(&_inverseViewProjectionMatrix);
	return _inverseViewProjectionMatrix;
}


void Camera::setClearColor(float r, float g, float b, float a)
{
	_clearColor.set(r, g, b, a);
	setDirty<DIRTY_BIT_CLEAR_COLOR>();
}


void Camera::update()
{
}


void Camera::render(const RenderContext& context)
{
	if (checkAndCleanBit<DIRTY_BIT_VIEWPORT>())
		applyViewportChange();
	if (checkAndCleanBit<DIRTY_BIT_CLEAR_COLOR>())
		applyClearColor();
	clear();
}