#include "SoloEngine.h"
#include "SoloNode.h"
#include "SoloDevice.h"
#include "SoloRenderTarget.h"
#include "platform/stub/SoloStubCamera.h"
#include "platform/opengl/SoloOpenGLCamera.h"

using namespace solo;


const unsigned DIRTY_BIT_VIEW = 1;
const unsigned DIRTY_BIT_PROJ = 2;
const unsigned DIRTY_BIT_VIEW_PROJ = 4;
const unsigned DIRTY_BIT_INV_VIEW = 8;
const unsigned DIRTY_BIT_INV_VIEW_PROJ = 16;
const unsigned DIRTY_BIT_ALL =
		DIRTY_BIT_VIEW |
		DIRTY_BIT_PROJ |
		DIRTY_BIT_VIEW_PROJ |
		DIRTY_BIT_INV_VIEW |
		DIRTY_BIT_INV_VIEW_PROJ;


shared<Camera> CameraFactory::create(EngineMode mode, Scene* scene, Node node)
{
	if (mode == EngineMode::OpenGL)
		return SL_NEW2(OpenGLCamera, scene, node);
	return SL_NEW2(StubCamera, scene, node);
}


Camera::Camera(Scene* scene, Node node):
	ComponentBase{ node },
	scene{ scene }
{
	renderTags.setAll();
}


void Camera::init()
{
	transform = node.getComponent<Transform>();
	transform->addCallback(this);
	auto canvasSize = scene->getEngine()->getDevice()->getCanvasSize();
	setAspectRatio(canvasSize.x / canvasSize.y);
	dirtyFlags.add(DIRTY_BIT_ALL); // arguably
}


void Camera::onTransformChanged(const Transform* transform)
{
	dirtyFlags.add(DIRTY_BIT_VIEW | DIRTY_BIT_VIEW_PROJ | DIRTY_BIT_INV_VIEW | DIRTY_BIT_INV_VIEW_PROJ);
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
	dirtyFlags.add(DIRTY_BIT_PROJ | DIRTY_BIT_VIEW_PROJ | DIRTY_BIT_INV_VIEW_PROJ);
}


void Camera::setFOV(float fov)
{
	this->fov = fov;
	dirtyFlags.add(DIRTY_BIT_PROJ | DIRTY_BIT_VIEW_PROJ | DIRTY_BIT_INV_VIEW_PROJ);
}


void Camera::setWidth(float width)
{
	this->width = width;
	dirtyFlags.add(DIRTY_BIT_PROJ | DIRTY_BIT_VIEW_PROJ |DIRTY_BIT_INV_VIEW_PROJ);
}


void Camera::setHeight(float height)
{
	this->height = height;
	dirtyFlags.add(DIRTY_BIT_PROJ | DIRTY_BIT_VIEW_PROJ | DIRTY_BIT_INV_VIEW_PROJ);
}


void Camera::setAspectRatio(float ratio)
{
	aspectRatio = ratio;
	dirtyFlags.add(DIRTY_BIT_PROJ | DIRTY_BIT_VIEW_PROJ | DIRTY_BIT_INV_VIEW_PROJ);
}


void Camera::setFar(float far)
{
	this->farClip = far;
	dirtyFlags.add(DIRTY_BIT_PROJ | DIRTY_BIT_VIEW_PROJ | DIRTY_BIT_INV_VIEW_PROJ);
}


void Camera::setNear(float near)
{
	this->nearClip = near;
	dirtyFlags.add(DIRTY_BIT_PROJ | DIRTY_BIT_VIEW_PROJ | DIRTY_BIT_INV_VIEW_PROJ);
}


const Matrix& Camera::getViewMatrix()
{
	if (dirtyFlags.checkAndRemove(DIRTY_BIT_VIEW))
	{
		viewMatrix = transform->getWorldMatrix();
		viewMatrix.invert();
	}
	return viewMatrix;
}


const Matrix& Camera::getInverseViewMatrix()
{
	if (dirtyFlags.checkAndRemove(DIRTY_BIT_INV_VIEW))
	{
		inverseViewMatrix = getViewMatrix();
		inverseViewMatrix.invert();
	}
	return inverseViewMatrix;
}


const Matrix& Camera::getProjectionMatrix()
{
	if (dirtyFlags.checkAndRemove(DIRTY_BIT_PROJ))
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
	if (dirtyFlags.checkAndRemove(DIRTY_BIT_VIEW_PROJ))
		viewProjectionMatrix = getProjectionMatrix() * getViewMatrix();
	return viewProjectionMatrix;
}


const Matrix& Camera::getInverseViewProjectionMatrix()
{
	if (dirtyFlags.checkAndRemove(DIRTY_BIT_INV_VIEW_PROJ))
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
