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

static auto toBlendFactor(BlendFactor factor) -> GLenum
{
    switch (factor)
    {
        case BlendFactor::Zero:
            return GL_ZERO;
        case BlendFactor::One:
            return GL_ONE;
        case BlendFactor::SrcColor:
            return GL_SRC_COLOR;
        case BlendFactor::OneMinusSrcColor:
            return GL_ONE_MINUS_SRC_COLOR;
        case BlendFactor::DstColor:
            return GL_DST_COLOR;
        case BlendFactor::OneMinusDstColor:
            return GL_ONE_MINUS_DST_COLOR;
        case BlendFactor::SrcAlpha:
            return GL_SRC_ALPHA;
        case BlendFactor::OneMinusSrcAlpha:
            return GL_ONE_MINUS_SRC_ALPHA;
        case BlendFactor::DstAlpha:
            return GL_DST_ALPHA;
        case BlendFactor::OneMinusDstAlpha:
            return GL_ONE_MINUS_DST_ALPHA;
        case BlendFactor::ConstantAlpha:
            return GL_CONSTANT_ALPHA;
        case BlendFactor::OneMinusConstantAlpha:
            return GL_ONE_MINUS_CONSTANT_ALPHA;
        case BlendFactor::SrcAlphaSaturate:
            return GL_SRC_ALPHA_SATURATE;
        default:
            SL_PANIC("Unknown blend factor");
            return 0;
    }
}

static void clear(bool color, bool depth, const Vector4 &clearColor)
{
    if (color)
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    const GLbitfield flags = (color ? GL_COLOR_BUFFER_BIT : 0) | (depth ? GL_DEPTH_BUFFER_BIT : 0);
    glClear(flags);
}

static void setViewport(const Vector4 &viewport)
{
    glViewport(viewport.x, viewport.y, viewport.z, viewport.w);
}

static void setDepthWrite(bool enabled)
{
    glDepthMask(enabled ? GL_TRUE : GL_FALSE);
}

static void setDepthTest(bool enabled)
{
    enabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

static void setDepthFunction(DepthFunction func)
{
    GLenum glfunc = 0;
    switch (func)
    {
        case DepthFunction::Never:
            glfunc = GL_NEVER;
            break;
        case DepthFunction::Less:
            glfunc = GL_LESS;
            break;
        case DepthFunction::Equal:
            glfunc = GL_EQUAL;
            break;
        case DepthFunction::LEqual:
            glfunc = GL_LEQUAL;
            break;
        case DepthFunction::Greater:
            glfunc = GL_GREATER;
            break;
        case DepthFunction::NotEqual:
            glfunc = GL_NOTEQUAL;
            break;
        case DepthFunction::GEqual:
            glfunc = GL_GEQUAL;
            break;
        case DepthFunction::Always:
            glfunc = GL_ALWAYS;
            break;
        default:
            break;
    }
    if (glfunc)
        glDepthFunc(glfunc);
}

static void setPolygonMode(PolygonMode mode)
{
    GLenum glMode;
    switch (mode)
    {
        case PolygonMode::Fill:
            glMode = GL_FILL;
            break;
        case PolygonMode::Wireframe:
            glMode = GL_LINE;
            break;
        case PolygonMode::Points:
            glMode = GL_POINT;
            break;
        default:
            return;
    }

    glPolygonMode(GL_FRONT_AND_BACK, glMode);
}

static void setFaceCull(FaceCull cull)
{
    switch (cull)
    {
        case FaceCull::None:
            glDisable(GL_CULL_FACE);
            break;
        case FaceCull::Back:
            glEnable(GL_CULL_FACE);
            glFrontFace(GL_CW);
            break;
        case FaceCull::Front:
            glEnable(GL_CULL_FACE);
            glFrontFace(GL_CCW);
            break;
        default:
            break;
    }
}

static void setBlend(bool enabled)
{
    enabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
}

static void setBlendFactor(BlendFactor srcFactor, BlendFactor dstFactor)
{
    glBlendFunc(toBlendFactor(srcFactor), toBlendFactor(dstFactor));
}

OpenGLRenderer::OpenGLRenderer(Device *device)
{
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    device->getLogger()->logInfo(SL_FMT("Running in OpenGL ", major, ".", minor, " mode"));
}

void OpenGLRenderer::addRenderCommand(const RenderCommand &cmd)
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

            GLuint fb = 0;
            auto target = step.cmd.camera->getRenderTarget();
            if (target)
                fb = static_cast<OpenGLFrameBuffer*>(target.get())->getHandle();

            step.beginCamera = [=]
            {
                if (fb)
                    glBindFramebuffer(GL_FRAMEBUFFER, fb);

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

        case RenderCommandType::DrawMesh:
        case RenderCommandType::DrawMeshPart:
        {
            const auto material = cmd.type == RenderCommandType::DrawMesh ? cmd.mesh.material : cmd.meshPart.material;
            const auto faceCull = material->getFaceCull();
            const auto polygonMode = material->getPolygonMode();
            const auto depthTest = material->getDepthTest();
            const auto depthWrite = material->getDepthWrite();
            const auto depthFunc = material->getDepthFunction();
            const auto blend = material->getBlend();
            const auto srcBlendFactor = material->getSrcBlendFactor();
            const auto dstBlendFactor = material->getDstBlendFactor();
            const auto effect = static_cast<OpenGLEffect*>(material->getEffect().get());
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

void OpenGLRenderer::beginFrame()
{
    renderSteps.clear();
}

// TODO Optimize: group by material etc.
// TODO Build "render plan", update it only when something has really changed
// TODO Avoid dynamic casts
// TODO Make it consistent whether ogl classes contain drawing code themselves or only serve as a source of data
// for the renderer
void OpenGLRenderer::endFrame()
{
    Camera *currentCamera = nullptr;

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
                step.applyMaterialState();
                static_cast<OpenGLMaterial*>(step.cmd.mesh.material)->applyParams(currentCamera, step.cmd.mesh.transform);
                const auto effect = static_cast<OpenGLEffect*>(step.cmd.mesh.material->getEffect().get());
                static_cast<OpenGLMesh*>(step.cmd.mesh.mesh)->draw(effect);
                break;
            }

            case RenderCommandType::DrawMeshPart:
            {
                step.applyMaterialState();
                static_cast<OpenGLMaterial*>(step.cmd.meshPart.material)->applyParams(currentCamera, step.cmd.meshPart.transform);
                const auto effect = static_cast<OpenGLEffect*>(step.cmd.meshPart.material->getEffect().get());
                static_cast<OpenGLMesh*>(step.cmd.meshPart.mesh)->drawPart(step.cmd.meshPart.part, effect);
                break;
            }

            default: break;
        }
    }
}

#endif
