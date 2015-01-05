#include "SoloCamera.h"
#include "SoloEngine.h"
#include "SoloVideoDriver.h"
#include "SoloDevice.h"

using namespace solo;

const int DIRTY_BIT_VIEWPORT = 1;
const int DIRTY_BIT_CLEAR_COLOR = 2;
const int DIRTY_BIT_ALL = DIRTY_BIT_VIEWPORT | DIRTY_BIT_CLEAR_COLOR;


Camera::Camera():
	_primary(false),
	_viewport(0, 0, 1, 1)
{
	_driver = PTR_CAST_STATIC<VideoDriver>(Engine::getEngine()->getVideoDriver());
	_device = PTR_CAST_STATIC<Device>(Engine::getEngine()->getDevice());
	setDirty<DIRTY_BIT_ALL>(); // arguably
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


void Camera::setClearColor(float r, float g, float b, float a)
{
	_clearColor.set(r, g, b, a);
	setDirty<DIRTY_BIT_CLEAR_COLOR>();
}


void Camera::setPrimary(bool primary)
{
	_primary = primary;
}


bool Camera::isPrimary() const
{
	return _primary;
}


void Camera::update()
{
}


void Camera::render()
{
	if (_primary)
	{
		if (checkBitAndClean<DIRTY_BIT_VIEWPORT>())
		{
			auto size = _device->getCanvasSize();
			_driver->setViewport(_viewport.x * size.x, _viewport.y * size.y, _viewport.z * size.x, _viewport.w * size.y);
		}
		if (checkBitAndClean<DIRTY_BIT_CLEAR_COLOR>())
			_driver->setClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w);
		_driver->clear();
	}
}