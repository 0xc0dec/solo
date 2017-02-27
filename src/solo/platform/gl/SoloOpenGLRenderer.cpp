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
#include <unordered_map>

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


static auto toGLCubeTextureFace(CubeTextureFace face) -> GLenum
{
    switch (face)
    {
        case CubeTextureFace::Front:
            return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
        case CubeTextureFace::Back:
            return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
        case CubeTextureFace::Right:
            return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
        case CubeTextureFace::Left:
            return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
        case CubeTextureFace::Top:
            return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
        case CubeTextureFace::Bottom:
            return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
        default:
            SL_PANIC("Unknown cube texture face");
            return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
    }
}


static auto toTextureFormat(TextureFormat format) -> GLenum
{
    switch (format)
    {
        case TextureFormat::Red:
            return GL_RED;
        case TextureFormat::RGB:
            return GL_RGB;
        case TextureFormat::RGBA:
            return GL_RGBA;
        case TextureFormat::Alpha:
            return GL_ALPHA;
        default:
            SL_PANIC("Unknown texture format");
            return GL_RED;
    }
}


static auto toInternalTextureFormat(TextureFormat format) -> GLenum
{
    switch (format)
    {
        case TextureFormat::Red:
        case TextureFormat::RGB:
            return GL_RGB;
        case TextureFormat::RGBA:
            return GL_RGBA;
        case TextureFormat::Alpha:
            return GL_ALPHA;
        default:
            SL_PANIC("Unknown texture format");
            return GL_RGB;
    }
}


gl::Renderer::Renderer(Device *device)
{
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    device->getLogger()->logInfo(SL_FMT("Running in OpenGL ", major, ".", minor, " mode"));
}


gl::Renderer::~Renderer()
{
    // All resources at this point should have already been released
    while (!textures.empty())
        destroyTexture(textures.begin()->first);
}


void gl::Renderer::addRenderCommand(const solo::RenderCommand &cmd)
{
    renderCommands.push_back(cmd);
}


auto gl::Renderer::createTexture() -> uint32_t
{
    GLuint rawHandle = 0;
    glGenTextures(1, &rawHandle);
    SL_PANIC_IF(!rawHandle, "Failed to obtain texture handle");

    auto handle = textureCounter++;
    textures[handle].rawHandle = rawHandle;

    return handle;
}


void gl::Renderer::destroyTexture(uint32_t handle)
{
    auto rawHandle = textures.at(handle).rawHandle;
    glDeleteTextures(1, &rawHandle);
    textures.erase(handle);
}


void gl::Renderer::updateRectTexture(uint32_t handle, TextureFormat format, uint32_t width, uint32_t height, const void *data)
{
    bindTexture(GL_TEXTURE_2D, handle);

    auto internalFormat = toInternalTextureFormat(format);
    auto fmt = toTextureFormat(format);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, fmt, GL_UNSIGNED_BYTE, data);

    auto &texture = textures.at(handle);
    texture.width = width;
    texture.height = height;

    bindTexture(GL_TEXTURE_2D, EmptyHandle);
}


void gl::Renderer::updateCubeTexture(uint32_t handle, CubeTextureFace face, TextureFormat format, uint32_t width, uint32_t height, const void *data)
{
    bindTexture(GL_TEXTURE_CUBE_MAP, handle);

    auto glFace = toGLCubeTextureFace(face);
    auto internalFormat = toInternalTextureFormat(format);
    auto fmt = toTextureFormat(format);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(glFace, 0, internalFormat, width, height, 0, fmt, GL_UNSIGNED_BYTE, data);

    // NB width and height in texture data are not updated intentionally

    bindTexture(GL_TEXTURE_CUBE_MAP, EmptyHandle);
}


void gl::Renderer::generateRectTextureMipmaps(uint32_t handle)
{
    bindTexture(GL_TEXTURE_2D, handle);
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glGenerateMipmap(GL_TEXTURE_2D);
    bindTexture(GL_TEXTURE_2D, EmptyHandle);
}


void gl::Renderer::generateCubeTextureMipmaps(uint32_t handle)
{
    bindTexture(GL_TEXTURE_CUBE_MAP, handle);
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    bindTexture(GL_TEXTURE_CUBE_MAP, EmptyHandle);
}


void gl::Renderer::bindTexture(GLenum target, uint32_t handle)
{
    auto rawHandle = handle == EmptyHandle ? 0 : textures.at(handle).rawHandle;
    glBindTexture(target, rawHandle);
}


void gl::Renderer::setTexture(GLenum target, uint32_t handle, uint32_t flags)
{
    bindTexture(target, handle);

    if (!flags || handle == EmptyHandle)
        return;

    GLenum minFilter = 0;
    if (flags & TextureFlags::MinFilterLinear)
        minFilter = GL_LINEAR;
    else if (flags & TextureFlags::MinFilterLinearMipmapLinear)
        minFilter = GL_LINEAR_MIPMAP_LINEAR;
    else if (flags & TextureFlags::MinFilterLinearMipmapNearest)
        minFilter = GL_LINEAR_MIPMAP_NEAREST;
    else if (flags & TextureFlags::MinFilterNearest)
        minFilter = GL_NEAREST;
    else if (flags & TextureFlags::MinFilterNearestMipmapLinear)
        minFilter = GL_NEAREST_MIPMAP_LINEAR;
    else if (flags & TextureFlags::MinFilterNearestMipmapNearest)
        minFilter = GL_NEAREST_MIPMAP_NEAREST;
    if (minFilter)
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);

    GLenum magFilter = 0;
    if (flags & TextureFlags::MagFilterLinear)
        magFilter = GL_LINEAR;
    else if (flags & TextureFlags::MagFilterLinearMipmapLinear)
        magFilter = GL_LINEAR_MIPMAP_LINEAR;
    else if (flags & TextureFlags::MagFilterLinearMipmapNearest)
        magFilter = GL_LINEAR_MIPMAP_NEAREST;
    else if (flags & TextureFlags::MagFilterNearest)
        magFilter = GL_NEAREST;
    else if (flags & TextureFlags::MagFilterNearestMipmapLinear)
        magFilter = GL_NEAREST_MIPMAP_LINEAR;
    else if (flags & TextureFlags::MagFilterNearestMipmapNearest)
        magFilter = GL_NEAREST_MIPMAP_NEAREST;
    if (magFilter)
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);

    GLenum wrapS = 0;
    if (flags & TextureFlags::HorizontalWrapClamp)
        wrapS = GL_CLAMP_TO_EDGE;
    else if (flags & TextureFlags::HorizontalWrapRepeat)
        wrapS = GL_REPEAT;
    if (wrapS)
        glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapS);

    GLenum wrapT = 0;
    if (flags & TextureFlags::VerticalWrapClamp)
        wrapT = GL_CLAMP_TO_EDGE;
    else if (flags & TextureFlags::VerticalWrapRepeat)
        wrapT = GL_REPEAT;
    if (wrapT)
        glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapT);
}


void gl::Renderer::setRectTexture(uint32_t handle)
{
    bindTexture(GL_TEXTURE_2D, handle);
}


void gl::Renderer::setRectTexture(uint32_t handle, uint32_t flags)
{
    setTexture(GL_TEXTURE_2D, handle, flags);
}


void gl::Renderer::setRectTexture(uint32_t handle, uint32_t flags, float anisotropyLevel)
{
    setRectTexture(handle, flags);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropyLevel);
}


void gl::Renderer::setCubeTexture(uint32_t handle)
{
    bindTexture(GL_TEXTURE_CUBE_MAP, handle);
}


void gl::Renderer::setCubeTexture(uint32_t handle, uint32_t flags)
{
    setTexture(GL_TEXTURE_CUBE_MAP, handle, flags);

    if (handle == EmptyHandle)
        return;

    GLenum wrapR = 0;
    if (flags & TextureFlags::DepthWrapClamp)
        wrapR = GL_CLAMP_TO_EDGE;
    else if (flags & TextureFlags::DepthWrapRepeat)
        wrapR = GL_REPEAT;
    if (wrapR)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapR);
}


void gl::Renderer::setCubeTexture(uint32_t handle, uint32_t flags, float anisotropyLevel)
{
    setCubeTexture(handle, flags);
    if (handle != EmptyHandle)
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropyLevel);
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
