#include <GL/glew.h>
#include "SoloOpenGLCamera.h"
#include "SoloEngine.h"
#include "SoloDevice.h"

using namespace solo;


ptr<OpenGLCamera> OpenGLCamera::create(size_t node)
{
	return NEW2(OpenGLCamera, node);
}


OpenGLCamera::OpenGLCamera(size_t node):
	Camera(node)
{
	_device = Engine::get()->getDevice();
}


void OpenGLCamera::applyViewportChange()
{
	auto size = _device->getCanvasSize();
	glViewport(
		static_cast<GLuint>(_viewport.x * size.x),
		static_cast<GLuint>(_viewport.y * size.y),
		static_cast<GLuint>(_viewport.z * size.x),
		static_cast<GLuint>(_viewport.w * size.y));
}


void OpenGLCamera::applyClearColor()
{
	glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w);
}


void OpenGLCamera::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}