//#include "SoloOpenGLCamera.h"
//#include "SoloDevice.h"
//#include <GL/glew.h>
//
//using namespace solo;
//
//
//OpenGLCamera::OpenGLCamera(Scene* scene, Node node):
//    Camera(scene, node)
//{
//}
//
//
//void OpenGLCamera::init()
//{
//    Camera::init();
//    device = scene->getDevice();
//}
//
//
//void OpenGLCamera::applyViewport()
//{
//    if (viewportSet)
//    {
//        glViewport(
//            static_cast<GLuint>(viewport.x),
//            static_cast<GLuint>(viewport.y),
//            static_cast<GLuint>(viewport.z),
//            static_cast<GLuint>(viewport.w));
//    }
//    else
//    {
//        auto size = device->getCanvasSize();
//        glViewport(
//            static_cast<GLuint>(0),
//            static_cast<GLuint>(0),
//            static_cast<GLuint>(size.x),
//            static_cast<GLuint>(size.y));
//    }
//}
//
//
//void OpenGLCamera::applyClearColor()
//{
//    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
//}
//
//
//void OpenGLCamera::clear()
//{
//    glDepthMask(GL_TRUE);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//}