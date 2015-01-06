#include <GL/glew.h>
#include "SoloOpenGLVideoDriver.h"

using namespace solo;


OpenGLVideoDriver::OpenGLVideoDriver()
{
}


OpenGLVideoDriver::~OpenGLVideoDriver()
{
}


ptr<OpenGLVideoDriver> OpenGLVideoDriver::create()
{
	return NEW2(OpenGLVideoDriver);
}


void OpenGLVideoDriver::setViewport(float left, float top, float width, float height)
{
	glViewport(static_cast<GLuint>(left), static_cast<GLuint>(top), static_cast<GLuint>(width), static_cast<GLuint>(height));
}


void OpenGLVideoDriver::setClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}


void OpenGLVideoDriver::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
