/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLRenderer.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloDevice.h"
#include "SoloCamera.h"
#include "SoloLogger.h"
#include "SoloOpenGLMaterial.h"
#include "SoloOpenGLMesh.h"
#include "SoloOpenGLFrameBuffer.h"
#include "SoloOpenGL.h"

using namespace solo;
using namespace gl;


gl::Renderer::Renderer(Device *device)
{
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    device->getLogger()->logInfo(SL_FMT("Running in OpenGL ", major, ".", minor, " mode"));
}


void gl::Renderer::addRenderCommand(const RenderCommand &cmd)
{
    renderCommands.push_back(cmd);
}


void gl::Renderer::beginFrame()
{
    renderCommands.clear();
}


// TODO Optimize: group by material etc.
// TODO Build "render plan", update it only when something has really changed
// TODO Avoid dynamic casts
// TODO Make it consistent whether ogl classes contain drawing code themselves or only serve as a source of data
// for the renderer
void gl::Renderer::endFrame()
{
    const Camera *currentCamera = nullptr;
    const Material *currentMaterial = nullptr;
    const FrameBuffer *currentFrameBuffer = nullptr;

    for (const auto &cmd: renderCommands)
    {
        switch (cmd.type)
        {
            case RenderCommandType::BeginCamera:
            {
                currentFrameBuffer = dynamic_cast<const FrameBuffer *>(cmd.camera.frameBuffer);
                if (currentFrameBuffer)
                    currentFrameBuffer->bind();

                auto cam = cmd.camera.camera;
                
                setViewport(cam->getViewport());
                setDepthWrite(true);
                setDepthTest(true);
                clear(cam->isClearColorEnabled(), cam->isClearDepthEnabled(), cam->getClearColor());

                currentCamera = dynamic_cast<const Camera*>(cam);

                break;
            }

            case RenderCommandType::EndCamera:
            {
                if (currentFrameBuffer)
                    currentFrameBuffer->unbind();
                currentFrameBuffer = nullptr;
                currentCamera = nullptr;
                break;
            }

            case RenderCommandType::DrawMesh:
            {
                currentMaterial->applyParams(currentCamera, cmd.mesh.transform);
                dynamic_cast<const Mesh*>(cmd.mesh.mesh)->draw();
                break;
            }

            case RenderCommandType::DrawMeshPart:
            {
                currentMaterial->applyParams(currentCamera, cmd.meshPart.transform);
                dynamic_cast<const Mesh*>(cmd.meshPart.mesh)->drawPart(cmd.meshPart.part);
                break;
            }

            case RenderCommandType::ApplyMaterial:
            {
                auto glEffect = dynamic_cast<Effect*>(cmd.material->getEffect());
                
                glUseProgram(glEffect->getHandle());
                
                auto glMaterial = dynamic_cast<const Material*>(cmd.material);
                glMaterial->applyState();

                currentMaterial = glMaterial;

                break;
            }

            default: break;
        }
    }
}


#endif
