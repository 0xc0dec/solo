#include "SoloEngine.h"
#include "SoloStubCamera.h"
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
const unsigned DIRTY_BIT_ALL =
		DIRTY_BIT_VIEW |
		DIRTY_BIT_PROJ |
		DIRTY_BIT_VIEW_PROJ |
		DIRTY_BIT_INV_VIEW |
		DIRTY_BIT_INV_VIEW_PROJ;


shared<Camera> CameraFactory::create(EngineMode mode, Scene* scene, Node node)
{
	if (mode == EngineMode::OpenGL)
		return NEW2(OpenGLCamera, scene, node);
	return NEW2(StubCamera, scene, node);
}


Camera::Camera(Scene* scene, Node node):
	ComponentBase{ node },
	scene{ scene }
{
}


void Camera::init()
{
	transform = node.getComponent<Transform>();
	transform->addCallback(this);
	auto canvasSize = scene->getEngine()->getDevice()->getCanvasSize();
	setAspectRatio(canvasSize.x / canvasSize.y);
	setDirty<DIRTY_BIT_ALL>(); // arguably
}


void Camera::terminate()
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
	{
		viewMatrix = transform->getWorldMatrix();
		viewMatrix.invert();
	}
	return viewMatrix;
}


const Matrix& Camera::getInverseViewMatrix()
{
	if (checkAndCleanBit<DIRTY_BIT_INV_VIEW>())
	{
		inverseViewMatrix = getViewMatrix();
		inverseViewMatrix.invert();
	}
	return inverseViewMatrix;
}


const Matrix& Camera::getProjectionMatrix()
{
	if (checkAndCleanBit<DIRTY_BIT_PROJ>())
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
	if (checkAndCleanBit<DIRTY_BIT_VIEW_PROJ>())
		viewProjectionMatrix = getProjectionMatrix() * getViewMatrix();
	return viewProjectionMatrix;
}


const Matrix& Camera::getInverseViewProjectionMatrix()
{
	if (checkAndCleanBit<DIRTY_BIT_INV_VIEW_PROJ>())
	{
		inverseViewProjectionMatrix = getViewProjectionMatrix();
		inverseViewProjectionMatrix.invert();
	}
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

