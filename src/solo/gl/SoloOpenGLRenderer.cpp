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

static void clear(bool color, const Vector4 &clearColor)
{
    if (color)
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    const GLbitfield flags = (color ? GL_COLOR_BUFFER_BIT : 0) | GL_DEPTH_BUFFER_BIT;
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

void OpenGLRenderer::beginCamera(Camera *camera, FrameBuffer *renderTarget)
{
    if (renderTarget)
    {
        const auto fb = static_cast<OpenGLFrameBuffer*>(renderTarget)->getHandle();
        glBindFramebuffer(GL_FRAMEBUFFER, fb);
    }
        
    const auto viewport = camera->getViewport();
    const auto hasClearColor = camera->hasColorClearing();
    const auto clearColor = camera->getClearColor();

    setViewport(viewport);
    setDepthWrite(true);
    setDepthTest(true);
    clear(hasClearColor, clearColor);

    currentCamera = camera;
}

// TODO pass render target, just as in beginCamera method
void OpenGLRenderer::endCamera(Camera *camera)
{
    if (camera->getRenderTarget())
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    currentCamera = nullptr;
}

void OpenGLRenderer::drawMesh(Mesh *mesh, Transform *transform, Material *material)
{
    const auto faceCull = material->getFaceCull();
    const auto polygonMode = material->getPolygonMode();
    const auto depthTest = material->hasDepthTest();
    const auto depthWrite = material->hasDepthWrite();
    const auto depthFunc = material->getDepthFunction();
    const auto blend = material->getBlend();
    const auto srcBlendFactor = material->getSrcBlendFactor();
    const auto dstBlendFactor = material->getDstBlendFactor();
    const auto effect = static_cast<OpenGLEffect*>(material->getEffect().get());
    const auto program = effect->getHandle();

    glUseProgram(program);
    setFaceCull(faceCull);
    setPolygonMode(polygonMode);
    setDepthTest(depthTest);
    setDepthWrite(depthWrite);
    setDepthFunction(depthFunc);
    setBlend(blend);
    setBlendFactor(srcBlendFactor, dstBlendFactor);

    static_cast<OpenGLMaterial*>(material)->applyParams(currentCamera, transform);
    static_cast<OpenGLMesh*>(mesh)->draw(effect);
}

void OpenGLRenderer::drawMeshPart(Mesh *mesh, u32 part, Transform *transform, Material *material)
{
    // TODO Remove copy-paste
    const auto faceCull = material->getFaceCull();
    const auto polygonMode = material->getPolygonMode();
    const auto depthTest = material->hasDepthTest();
    const auto depthWrite = material->hasDepthWrite();
    const auto depthFunc = material->getDepthFunction();
    const auto blend = material->getBlend();
    const auto srcBlendFactor = material->getSrcBlendFactor();
    const auto dstBlendFactor = material->getDstBlendFactor();
    const auto effect = static_cast<OpenGLEffect*>(material->getEffect().get());
    const auto program = effect->getHandle();

    glUseProgram(program);
    setFaceCull(faceCull);
    setPolygonMode(polygonMode);
    setDepthTest(depthTest);
    setDepthWrite(depthWrite);
    setDepthFunction(depthFunc);
    setBlend(blend);
    setBlendFactor(srcBlendFactor, dstBlendFactor);

    static_cast<OpenGLMaterial*>(material)->applyParams(currentCamera, transform);
    static_cast<OpenGLMesh*>(mesh)->drawPart(part, effect);
}

void OpenGLRenderer::beginFrame()
{
    currentCamera = nullptr;
}

void OpenGLRenderer::endFrame()
{
}

#endif
