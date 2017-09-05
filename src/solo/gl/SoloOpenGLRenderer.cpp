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

    switch (cmd.type)
    {
        case RenderCommandType::BeginCamera:
        {
            const auto viewport = cmd.camera->getViewport();
            const auto colorClearEnabled = cmd.camera->isClearColorEnabled();
            const auto depthClearEnabled = cmd.camera->isClearDepthEnabled();
            const auto clearColor = cmd.camera->getClearColor();

            GLuint targetFBHandle = 0;
            auto target = step.cmd.camera->getRenderTarget();
            if (target)
            {
                if (target)
                    targetFBHandle = static_cast<FrameBuffer*>(target.get())->getHandle();
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

            break;
        }

        case RenderCommandType::EndCamera:
        {
            const auto hasTarget = step.cmd.camera->getRenderTarget() != nullptr;

            step.endCamera = [=]
            {
                // Note: this assumes that the camera's frame buffer is the current one,
                // so we can correctly unbind it.
                if (hasTarget)
                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
            };

            break;
        }

        case RenderCommandType::ApplyMaterial:
        {
            const auto faceCull = cmd.material->getFaceCull();
            const auto polygonMode = cmd.material->getPolygonMode();
            const auto depthTest = cmd.material->getDepthTest();
            const auto depthWrite = cmd.material->getDepthWrite();
            const auto depthFunc = cmd.material->getDepthFunction();
            const auto blend = cmd.material->getBlend();
            const auto srcBlendFactor = cmd.material->getSrcBlendFactor();
            const auto dstBlendFactor = cmd.material->getDstBlendFactor();
            const auto effect = static_cast<Effect*>(step.cmd.material->getEffect());
            const auto program = effect->getHandle();

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

            break;
        }

        default: break;
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
    Camera *currentCamera = nullptr;
    Material *currentMaterial = nullptr;

    for (const auto &step: renderSteps)
    {
        switch (step.cmd.type)
        {
            case RenderCommandType::BeginCamera:
            {
                step.beginCamera();
                currentCamera = static_cast<Camera*>(step.cmd.camera);
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
                static_cast<const Mesh*>(step.cmd.mesh.mesh)->draw();
                break;
            }

            case RenderCommandType::DrawMeshPart:
            {
                currentMaterial->applyParams(currentCamera, step.cmd.meshPart.transform);
                static_cast<Mesh*>(step.cmd.meshPart.mesh)->drawPart(step.cmd.meshPart.part);
                break;
            }

            case RenderCommandType::ApplyMaterial:
            {
                step.applyMaterialState();
                currentMaterial = static_cast<Material*>(step.cmd.material);
                break;
            }

            default: break;
        }
    }
}

#endif
