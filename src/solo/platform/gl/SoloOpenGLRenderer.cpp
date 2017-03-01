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
    RenderStep step;
    step.cmd = cmd;

    if (cmd.type == RenderCommandType::BeginCamera)
    {
        auto viewport = cmd.camera->getViewport();
        auto colorClearEnabled = cmd.camera->isClearColorEnabled();
        auto depthClearEnabled = cmd.camera->isClearDepthEnabled();
        auto clearColor = cmd.camera->getClearColor();

        GLuint targetFBHandle = 0;
        auto target = step.cmd.camera->getRenderTarget();
        if (target)
        {
            if (target)
                targetFBHandle = dynamic_cast<const FrameBuffer *>(target.get())->getHandle();
        }

        step.beginCamera = [=]
        {
            if (targetFBHandle)
                glBindFramebuffer(GL_FRAMEBUFFER, targetFBHandle);

            setViewport(viewport);
            setDepthWrite(true);
            setDepthTest(true);
            clear(colorClearEnabled, depthClearEnabled, clearColor);
        };
    }
    else if (cmd.type == RenderCommandType::EndCamera)
    {
        auto hasTarget = step.cmd.camera->getRenderTarget() != nullptr;

        step.endCamera = [=]
        {
            // Note: this assumes that the camera's frame buffer is the current one,
            // so we can correctly unbind it.
            if (hasTarget)
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
        };
    }
    else if (cmd.type == RenderCommandType::ApplyMaterial)
    {
        auto faceCull = cmd.material->getFaceCull();
        auto polygonMode = cmd.material->getPolygonMode();
        auto depthTest = cmd.material->getDepthTest();
        auto depthWrite = cmd.material->getDepthWrite();
        auto depthFunc = cmd.material->getDepthFunction();
        auto blend = cmd.material->getBlend();
        auto srcBlendFactor = cmd.material->getSrcBlendFactor();
        auto dstBlendFactor = cmd.material->getDstBlendFactor();
        auto effect = dynamic_cast<Effect*>(step.cmd.material->getEffect());
        auto program = effect->getHandle();

        step.applyMaterialState = [=]
        {
            glUseProgram(program);
            setFaceCull(faceCull);
            setPolygonMode(polygonMode);
            setDepthTest(depthTest);
            setDepthWrite(depthWrite);
            setDepthFunction(depthFunc);
            setBlend(blend);
            setBlendFactor(srcBlendFactor, dstBlendFactor);
        };
    }

    renderSteps.push_back(step);
}


void gl::Renderer::beginFrame()
{
    renderSteps.clear();
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

    for (const auto &step: renderSteps)
    {
        switch (step.cmd.type)
        {
            case RenderCommandType::BeginCamera:
            {
                step.beginCamera();
                currentCamera = dynamic_cast<const Camera*>(step.cmd.camera);
                break;
            }

            case RenderCommandType::EndCamera:
            {
                step.endCamera();
                currentCamera = nullptr;
                break;
            }

            case RenderCommandType::DrawMesh:
            {
                currentMaterial->applyParams(currentCamera, step.cmd.mesh.transform);
                dynamic_cast<const Mesh*>(step.cmd.mesh.mesh)->draw();
                break;
            }

            case RenderCommandType::DrawMeshPart:
            {
                currentMaterial->applyParams(currentCamera, step.cmd.meshPart.transform);
                dynamic_cast<const Mesh*>(step.cmd.meshPart.mesh)->drawPart(step.cmd.meshPart.part);
                break;
            }

            case RenderCommandType::ApplyMaterial:
            {
                step.applyMaterialState();
                currentMaterial = dynamic_cast<const Material*>(step.cmd.material);
                break;
            }

            default: break;
        }
    }
}


#endif
