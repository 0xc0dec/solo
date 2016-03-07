#include "SoloOpenGLRenderer.h"
#include "SoloOpenGLHelper.h"
#include <algorithm>

using namespace solo;


OpenGLRenderer::OpenGLRenderer()
{
}


OpenGLRenderer::~OpenGLRenderer()
{
    // At this point all resources should already have been released

    // TODO DRY? Never heard of it
    while (frameBuffers.getHandleCount() > 0)
    {
        auto handle = frameBuffers.getHandle(0);
        auto data = frameBuffers.getData(handle);
        glDeleteFramebuffers(1, &data.rawHandle);
        frameBuffers.releaseHandle(handle);
    }

    while (vertexBuffers.getHandleCount() > 0)
    {
        auto handle = vertexBuffers.getHandle(0);
        auto data = vertexBuffers.getData(handle);
        glDeleteBuffers(1, &data.rawHandle);
        vertexBuffers.releaseHandle(handle);
    }

    while (indexBuffers.getHandleCount() > 0)
    {
        auto handle = indexBuffers.getHandle(0);
        auto data = indexBuffers.getData(handle);
        glDeleteBuffers(1, &data.rawHandle);
        indexBuffers.releaseHandle(handle);
    }

    while (textures.getHandleCount() > 0)
    {
        auto handle = textures.getHandle(0);
        auto data = textures.getData(handle);
        glDeleteTextures(1, &data.rawHandle);
        textures.releaseHandle(handle);
    }
}


TextureHandle OpenGLRenderer::createTexture()
{
    GLuint rawHandle = 0;
    glGenTextures(1, &rawHandle);
    SL_THROW_IF(rawHandle == 0, InternalException, "Failed to obtain texture handle")

    TextureHandle handle;
    handle.value = textures.reserveHandle();

    textures.getData(handle.value).rawHandle = rawHandle;

    return handle;
}


void OpenGLRenderer::destroyTexture(TextureHandle handle)
{
    SL_THROW_IF(handle.empty(), InvalidInputException, "Texture handle is empty")
    auto rawHandle = handle.empty() ? 0 : textures.getData(handle.value).rawHandle;
    glDeleteTextures(1, &rawHandle);
    textures.releaseHandle(handle.value);
}


void OpenGLRenderer::update2DTexture(TextureHandle handle, ColorFormat format, int width, int height, const std::vector<uint8_t>& data)
{
    SL_THROW_IF(width <= 0 || height <= 0, InvalidInputException, "Invalid texture dimensions")
    SL_THROW_IF(handle.empty(), InvalidInputException, "Texture handle is empty")

    bindTexture(GL_TEXTURE_2D, handle);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        OpenGLHelper::convertColorFormat(format),
        width,
        height,
        0,
        OpenGLHelper::convertColorFormat(format),
        GL_UNSIGNED_BYTE,
        data.size() ? data.data() : 0);

    auto& texData = textures.getData(handle.value);
    texData.width = width;
    texData.height = height;

    bindTexture(GL_TEXTURE_2D, EmptyTextureHandle);
}


void OpenGLRenderer::updateCubeTexture(TextureHandle handle, CubeTextureFace face, ColorFormat format, int width, int height, const std::vector<uint8_t>& data)
{
    SL_THROW_IF(width <= 0 || height <= 0, InvalidInputException, "Invalid texture dimensions")
    SL_THROW_IF(handle.empty(), InvalidInputException, "Texture handle is empty")

    bindTexture(GL_TEXTURE_CUBE_MAP, handle);

    auto glFace = OpenGLHelper::convertCubeTextureFace(face);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(
        glFace,
        0,
        OpenGLHelper::convertColorFormat(format),
        width,
        height,
        0,
        OpenGLHelper::convertColorFormat(format),
        GL_UNSIGNED_BYTE,
        data.size() ? data.data() : 0);

    // TODO width and height in texture data are not updated intentionally

    bindTexture(GL_TEXTURE_CUBE_MAP, EmptyTextureHandle);
}


void OpenGLRenderer::bindTexture(GLenum target, TextureHandle handle)
{
    auto rawHandle = handle.empty() ? 0 : textures.getData(handle.value).rawHandle;
    glBindTexture(target, rawHandle);
}


void OpenGLRenderer::bindVertexBuffer(VertexBufferHandle handle)
{
    auto rawHandle = handle.empty() ? 0 : vertexBuffers.getData(handle.value).rawHandle;
    glBindBuffer(GL_ARRAY_BUFFER, rawHandle);
}


void OpenGLRenderer::bindIndexBuffer(IndexBufferHandle handle)
{
    auto rawHandle = handle.empty() ? 0 : indexBuffers.getData(handle.value).rawHandle;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rawHandle);
}


void OpenGLRenderer::setTexture(GLenum target, TextureHandle handle, int flags)
{
    bindTexture(target, handle);

    if (!flags || handle.empty())
        return;

    GLenum minFilter = 0;
    if (flags & MinFilterLinear)
        minFilter = GL_LINEAR;
    else if (flags & MinFilterLinearMipmapLinear)
        minFilter = GL_LINEAR_MIPMAP_LINEAR;
    else if (flags & MinFilterLinearMipmapNearest)
        minFilter = GL_LINEAR_MIPMAP_NEAREST;
    else if (flags & MinFilterNearest)
        minFilter = GL_NEAREST;
    else if (flags & MinFilterNearestMipmapLinear)
        minFilter = GL_NEAREST_MIPMAP_LINEAR;
    else if (flags & MinFilterNearestMipmapNearest)
        minFilter = GL_NEAREST_MIPMAP_NEAREST;
    if (minFilter)
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);

    GLenum magFilter = 0;
    if (flags & MagFilterLinear)
        magFilter = GL_LINEAR;
    else if (flags & MagFilterLinearMipmapLinear)
        magFilter = GL_LINEAR_MIPMAP_LINEAR;
    else if (flags & MagFilterLinearMipmapNearest)
        magFilter = GL_LINEAR_MIPMAP_NEAREST;
    else if (flags & MagFilterNearest)
        magFilter = GL_NEAREST;
    else if (flags & MagFilterNearestMipmapLinear)
        magFilter = GL_NEAREST_MIPMAP_LINEAR;
    else if (flags & MagFilterNearestMipmapNearest)
        magFilter = GL_NEAREST_MIPMAP_NEAREST;
    if (magFilter)
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);

    GLenum wrapS = 0;
    if (flags & HorizontalWrapClamp)
        wrapS = GL_CLAMP_TO_BORDER;
    else if (flags & HorizontalWrapRepeat)
        wrapS = GL_REPEAT;
    if (wrapS)
        glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapS);

    GLenum wrapT = 0;
    if (flags & VerticalWrapClamp)
        wrapT = GL_CLAMP_TO_BORDER;
    else if (flags & VerticalWrapRepeat)
        wrapT = GL_REPEAT;
    if (wrapT)
        glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapT);
}


void OpenGLRenderer::validateFrameBufferAttachments(const std::vector<TextureHandle> attachments)
{
    SL_THROW_IF(attachments.size() > GL_MAX_COLOR_ATTACHMENTS, InvalidInputException, "Too many frame buffer attachments")

    auto width = -1, height = -1;
    for (auto i = 0; i < attachments.size(); i++)
    {
        auto data = textures.getData(attachments[i].value);
        if (width < 0)
        {
            width = data.width;
            height = data.height;
        }
        else
        {
            if (data.width != width || data.height != height)
                SL_THROW(InvalidInputException, "Frame buffer attachments must have the same size");
        }
    }
}


void OpenGLRenderer::set2DTexture(TextureHandle handle)
{
    bindTexture(GL_TEXTURE_2D, handle);
}


void OpenGLRenderer::set2DTexture(TextureHandle handle, int flags)
{
    setTexture(GL_TEXTURE_2D, handle, flags);
}


void OpenGLRenderer::set2DTexture(TextureHandle handle, int flags, float anisotropyLevel)
{
    set2DTexture(handle, flags);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropyLevel);
}


void OpenGLRenderer::setCubeTexture(TextureHandle handle)
{
    bindTexture(GL_TEXTURE_CUBE_MAP, handle);
}


void OpenGLRenderer::setCubeTexture(TextureHandle handle, int flags)
{
    setTexture(GL_TEXTURE_CUBE_MAP, handle, flags);

    if (handle.empty())
        return;

    GLenum wrapR = 0;
    if (flags & DepthWrapClamp)
        wrapR = GL_CLAMP_TO_BORDER;
    else if (flags & DepthWrapRepeat)
        wrapR = GL_REPEAT;
    if (wrapR)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapR);
}


void OpenGLRenderer::setCubeTexture(TextureHandle handle, int flags, float anisotropyLevel)
{
    setCubeTexture(handle, flags);
    if (!handle.empty())
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropyLevel);
}


void OpenGLRenderer::bindFrameBuffer(FrameBufferHandle handle)
{
    auto rawHandle = handle.empty() ? 0 : frameBuffers.getData(handle.value).rawHandle;
    glBindFramebuffer(GL_FRAMEBUFFER, rawHandle);
}


FrameBufferHandle OpenGLRenderer::createFrameBuffer()
{
    GLuint rawHandle = 0;
    glGenFramebuffers(1, &rawHandle);
    SL_THROW_IF(rawHandle == 0, InternalException, "Failed to obtain frame buffer handle")

    FrameBufferHandle handle;
    handle.value = frameBuffers.reserveHandle();
    frameBuffers.getData(handle.value).rawHandle = rawHandle;

    return handle;
}


void OpenGLRenderer::destroyFrameBuffer(FrameBufferHandle handle)
{
    SL_THROW_IF(handle.empty(), InvalidInputException, "Frame buffer handle is empty")
    auto rawHandle = handle.empty() ? 0 : frameBuffers.getData(handle.value).rawHandle;
    glDeleteFramebuffers(1, &rawHandle);
    // TODO release depth buffer?
    frameBuffers.releaseHandle(handle.value);
}


void OpenGLRenderer::setFrameBuffer(FrameBufferHandle handle)
{
    bindFrameBuffer(handle);
}


void OpenGLRenderer::updateFrameBuffer(FrameBufferHandle handle, const std::vector<TextureHandle> attachments)
{
    SL_THROW_IF(handle.empty(), InvalidInputException, "Frame buffer handle is empty")
    SL_MAYBE(validateFrameBufferAttachments(attachments))

    bindFrameBuffer(handle);

    auto data = frameBuffers.getData(handle.value);

    if (data.depthBufferHandle)
    {
        glDeleteRenderbuffers(1, &data.depthBufferHandle);
        data.depthBufferHandle = 0;
    }

    auto newCount = attachments.size();
    auto stepCount = std::max(newCount, static_cast<size_t>(data.attachmentCount));
    for (auto i = 0; i < stepCount; i++)
    {
        auto rawHandle = i < newCount ? textures.getData(attachments[i].value).rawHandle : 0;
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, rawHandle, 0);
    }

    if (newCount > 0)
    {
        // Re-create the depth buffer
        glGenRenderbuffers(1, &data.depthBufferHandle);
        SL_THROW_IF(!data.depthBufferHandle, InternalException, "Failed to obtain depth buffer handle")

        glBindRenderbuffer(GL_RENDERBUFFER, data.depthBufferHandle);
        auto firstAttachmentData = textures.getData(attachments[0].value);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, firstAttachmentData.width, firstAttachmentData.height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, data.depthBufferHandle);

        SL_THROW_IF(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE,
            InternalException, "Render target has invalid state")
    }

    bindFrameBuffer(EmptyFrameBufferHandle);
}


VertexBufferHandle OpenGLRenderer::createVertexBuffer(const VertexBufferLayout& layout, const void* data, int vertexCount)
{
    GLuint rawHandle = 0;
    glGenBuffers(1, &rawHandle);
    SL_THROW_IF(rawHandle == 0, InternalException, "Failed to obtain vertex buffer handle")

    VertexBufferHandle handle;
    handle.value = vertexBuffers.reserveHandle();
    vertexBuffers.getData(handle.value).rawHandle = rawHandle;

    bindVertexBuffer(handle);
    glBufferData(GL_ARRAY_BUFFER, layout.getSize() * vertexCount, data, GL_STATIC_DRAW);
    bindVertexBuffer(EmptyVertexBufferHandle);

    return handle;
}


void OpenGLRenderer::destroyVertexBuffer(VertexBufferHandle handle)
{
    SL_THROW_IF(handle.empty(), InvalidInputException, "Vertex buffer handle is empty")
    auto rawHandle = handle.empty() ? 0 : vertexBuffers.getData(handle.value).rawHandle;
    glDeleteBuffers(1, &rawHandle);
    vertexBuffers.releaseHandle(handle.value);
}


IndexBufferHandle OpenGLRenderer::createIndexBuffer(const void* data, int elementSize, int elementCount)
{
    GLuint rawHandle = 0;
    glGenBuffers(1, &rawHandle);
    SL_THROW_IF(rawHandle == 0, InternalException, "Failed to obtain index buffer handle")

    IndexBufferHandle handle;
    handle.value = indexBuffers.reserveHandle();
    indexBuffers.getData(handle.value).rawHandle = rawHandle;

    bindIndexBuffer(handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementSize * elementCount, data, GL_STATIC_DRAW);
    bindIndexBuffer(EmptyIndexBufferHandle);

    return handle;
}


void OpenGLRenderer::destroyIndexBuffer(IndexBufferHandle handle)
{
    SL_THROW_IF(handle.empty(), InvalidInputException, "Index buffer handle is empty")
    auto rawHandle = handle.empty() ? 0 : indexBuffers.getData(handle.value).rawHandle;
    glDeleteBuffers(1, &rawHandle);
    indexBuffers.releaseHandle(handle.value);
}


static GLint compileShader(GLuint type, const char* src)
{
    // TODO check source

    static std::unordered_map<GLuint, std::string> typeNames =
    {
        { GL_VERTEX_SHADER, "vertex" },
        { GL_FRAGMENT_SHADER, "fragment" }
    };

    auto shader = glCreateShader(type);

    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        int logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> log(logLength);
        glGetShaderInfoLog(shader, logLength, nullptr, log.data());
        glDeleteShader(shader);
        SL_THROW(EffectCompilationException, SL_FMT("Failed to compile ", typeNames[type], " shader"), log.data()); // TODO turn into debug-only exception
    }

    return shader;
}


static GLint createProgram(GLuint vs, GLuint fs)
{
    auto program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    int status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        int logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> log(logLength);
        glGetProgramInfoLog(program, logLength, nullptr, log.data());
        glDeleteProgram(program);
        SL_THROW(EffectCompilationException, "Failed to link program", log.data()); // TODO turn into debug-only exception
    }

    return program;
}


ProgramHandle OpenGLRenderer::createProgram(const char* vsSrc, const char* fsSrc)
{
    auto vs = compileShader(GL_VERTEX_SHADER, vsSrc);
    auto fs = compileShader(GL_FRAGMENT_SHADER, fsSrc);
    auto program = ::createProgram(vs, fs);

    glDetachShader(program, vs);
    glDeleteShader(vs);
    glDetachShader(program, fs);
    glDeleteShader(fs);

    ProgramHandle handle;
    handle.value = programs.reserveHandle();
    programs.getData(handle.value).rawHandle = program;

    return handle;
}


void OpenGLRenderer::destroyProgram(ProgramHandle handle)
{
    SL_THROW_IF(handle.empty(), InvalidInputException, "Program handle is empty")
    auto rawHandle = handle.empty() ? 0 : programs.getData(handle.value).rawHandle;
    glDeleteProgram(rawHandle);
    programs.releaseHandle(handle.value);
}


void OpenGLRenderer::setProgram(ProgramHandle handle)
{
    auto rawHandle = handle.empty() ? 0 : programs.getData(handle.value).rawHandle;
    glUseProgram(rawHandle);
}


void OpenGLRenderer::render()
{
    
}
