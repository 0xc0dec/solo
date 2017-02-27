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
#include <algorithm>

using namespace solo;
using namespace gl;


static auto toGLBlendFactor(BlendFactor factor) -> GLenum
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


gl::Renderer::Renderer(Device *device)
{
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    device->getLogger()->logInfo(SL_FMT("Running in OpenGL ", major, ".", minor, " mode"));
}


void gl::Renderer::addRenderCommand(const solo::RenderCommand &cmd)
{
    renderCommands.push_back(cmd);
}


void gl::Renderer::beginFrame()
{
    renderCommands.clear();
}


// TODO optimize: group by material etc.
// TODO build "render plan", update it only when something has really changed
// TODO avoid dynamic casts
void gl::Renderer::endFrame()
{
    const Camera *currentCamera = nullptr;
    const Material *currentMaterial = nullptr;
    const gl::FrameBuffer *currentFrameBuffer = nullptr;

    for (const auto &cmd: renderCommands)
    {
        switch (cmd.type)
        {
            case RenderCommandType::BeginCamera:
            {
                currentFrameBuffer = dynamic_cast<const gl::FrameBuffer *>(cmd.camera.frameBuffer);
                if (currentFrameBuffer)
                    currentFrameBuffer->bind();

                auto cam = cmd.camera.camera;
                auto viewport = cam->getViewport();
                setViewport(viewport.x, viewport.y, viewport.z, viewport.w);

                setDepthWrite(true);
                setDepthTest(true);
                
                auto color = cam->getClearColor();
                clear(cam->isClearColorEnabled(), cam->isClearDepthEnabled(), color.x, color.y, color.z, color.w);

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
                
                // TODO replace with Effect::apply() or smth, this is not consistent with
                // material application, for instance
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


void gl::Renderer::setDepthWrite(bool enabled)
{
    glDepthMask(enabled ? GL_TRUE : GL_FALSE);
}


void gl::Renderer::setDepthTest(bool enabled)
{
    enabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}


void gl::Renderer::setDepthFunction(DepthFunction func)
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


void gl::Renderer::setBlend(bool enabled)
{
    enabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
}


void gl::Renderer::setBlendFactor(BlendFactor srcFactor, BlendFactor dstFactor)
{
    glBlendFunc(toGLBlendFactor(srcFactor), toGLBlendFactor(dstFactor));
}


void gl::Renderer::setFaceCull(FaceCull cull)
{
    switch (cull)
    {
        case FaceCull::All:
            glDisable(GL_CULL_FACE);
            break;
        case FaceCull::CW:
            glEnable(GL_CULL_FACE);
            glFrontFace(GL_CW);
            break;
        case FaceCull::CCW:
            glEnable(GL_CULL_FACE);
            glFrontFace(GL_CCW);
            break;
        default:
            break;
    }
}


void gl::Renderer::setPolygonMode(PolygonMode mode)
{
    GLenum glMode;
    switch (mode)
    {
    case PolygonMode::Triangle:
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


void gl::Renderer::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glViewport(x, y, width, height);
}


void gl::Renderer::clear(bool color, bool depth, float r, float g, float b, float a)
{
    if (color)
        glClearColor(r, g, b, a);
    GLbitfield flags = (color ? GL_COLOR_BUFFER_BIT : 0) | (depth ? GL_DEPTH_BUFFER_BIT : 0);
    glClear(flags);
}

#endif
