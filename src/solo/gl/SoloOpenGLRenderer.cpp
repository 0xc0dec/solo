/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloOpenGLRenderer.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloDevice.h"
#include "SoloCamera.h"
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
    }

    SL_DEBUG_PANIC(true, "Unsupported blend factor");
    return 0;
}

static void clear(bool color, const Vector4 &clearColor)
{
    if (color)
        glClearColor(clearColor.x(), clearColor.y(), clearColor.z(), clearColor.w());
    const GLbitfield flags = (color ? GL_COLOR_BUFFER_BIT : 0) | GL_DEPTH_BUFFER_BIT;
    glClear(flags);
}

static void setViewport(const Vector4 &viewport)
{
    glViewport(viewport.x(), viewport.y(), viewport.z(), viewport.w());
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
            glFrontFace(GL_CCW);
            break;
        case FaceCull::Front:
            glEnable(GL_CULL_FACE);
            glFrontFace(GL_CW);
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

static auto version() -> std::pair<GLint, GLint>
{
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    return {major, minor};
}

void applyMaterial(Material *material)
{
    const auto effect = dynamic_cast<OpenGLEffect*>(material->effect().get());
    glUseProgram(static_cast<const GLuint>(effect->handle()));
    setFaceCull(material->faceCull());
    setPolygonMode(material->polygonMode());
    setDepthTest(material->hasDepthTest());
    setDepthWrite(material->hasDepthWrite());
    setDepthFunction(material->depthFunction());
    setBlend(material->hasBlend());
    setBlendFactor(material->srcBlendFactor(), material->dstBlendFactor());
}

OpenGLRenderer::OpenGLRenderer(Device *device)
{
	const auto ver = version();
    name_ = SL_FMT("OpenGL ", ver.first, ".", ver.second);
    SL_DEBUG_PANIC(!GLEW_VERSION_4_1, "Min supported OpenGL version is 4.1, this device supports ", ver.first, ".", ver.second);
}

auto OpenGLRenderer::gpuName() const -> const char*
{
    return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
}

void OpenGLRenderer::beginCamera(Camera *camera)
{
	const auto renderTarget = camera->renderTarget();
    if (renderTarget)
    {
        const auto fb = dynamic_cast<OpenGLFrameBuffer*>(renderTarget.get())->handle();
        glBindFramebuffer(GL_FRAMEBUFFER, fb);
    }

    setViewport(camera->viewport());
    setDepthWrite(true);
    setDepthTest(true);
    clear(camera->hasColorClearing(), camera->clearColor());

    currentCamera_ = camera;
}

void OpenGLRenderer::endCamera(Camera *camera)
{
	const auto renderTarget = camera->renderTarget();
    if (renderTarget)
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    currentCamera_ = nullptr;
}

void OpenGLRenderer::drawMesh(Mesh *mesh, Transform *transform, Material *material)
{
    applyMaterial(material);
    const auto effect = dynamic_cast<OpenGLEffect*>(material->effect().get());
    dynamic_cast<OpenGLMaterial*>(material)->applyParams(currentCamera_, transform);
    dynamic_cast<OpenGLMesh*>(mesh)->draw(effect);
}

void OpenGLRenderer::drawMeshPart(Mesh *mesh, u32 part, Transform *transform, Material *material)
{
    applyMaterial(material);
    const auto effect = dynamic_cast<OpenGLEffect*>(material->effect().get());
    dynamic_cast<OpenGLMaterial*>(material)->applyParams(currentCamera_, transform);
    dynamic_cast<OpenGLMesh*>(mesh)->drawPart(part, effect);
}

void OpenGLRenderer::beginFrame()
{
    currentCamera_ = nullptr;
}

void OpenGLRenderer::endFrame()
{
}

#endif
