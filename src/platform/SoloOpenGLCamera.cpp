#include "SoloOpenGLCamera.h"
#include "SoloEngine.h"
#include "SoloDevice.h"
#include <GL/glew.h>

using namespace solo;


OpenGLCamera::OpenGLCamera(Scene* scene, size_t nodeId) :
	Camera(scene, nodeId)
{
	device = Engine::get()->getDevice();
}


void OpenGLCamera::applyViewport()
{
	if (viewportSet)
	{
		glViewport(
			static_cast<GLuint>(viewport.x),
			static_cast<GLuint>(viewport.y),
			static_cast<GLuint>(viewport.z),
			static_cast<GLuint>(viewport.w));
	}
	else
	{
		auto size = device->getCanvasSize();
		glViewport(
			static_cast<GLuint>(0),
			static_cast<GLuint>(0),
			static_cast<GLuint>(size.x),
			static_cast<GLuint>(size.y));
	}
}


void OpenGLCamera::applyClearColor()
{
	glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
}


void OpenGLCamera::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}