#include <GL/glew.h>
#include "SoloOpenGLCamera.h"
#include "SoloEngine.h"
#include "SoloDevice.h"

using namespace solo;


OpenGLCamera::OpenGLCamera(Node* node):
	Camera(node)
{
	device = Engine::get()->getDevice();
}


void OpenGLCamera::applyViewportChange()
{
	auto size = device->getCanvasSize();
	glViewport(
		static_cast<GLuint>(viewport.x * size.x),
		static_cast<GLuint>(viewport.y * size.y),
		static_cast<GLuint>(viewport.z * size.x),
		static_cast<GLuint>(viewport.w * size.y));
}


void OpenGLCamera::applyClearColor()
{
	glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
}


void OpenGLCamera::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}