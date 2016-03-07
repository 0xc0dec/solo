#include "SoloOpenGLRenderer.h"
#include "SoloOpenGLHelper.h"
#include <algorithm>

using namespace solo;


static GLenum convertPrimitiveType(PrimitiveType type)
{
    switch (type)
    {
        case PrimitiveType::Triangles:
            return GL_TRIANGLES;
        case PrimitiveType::TriangleStrip:
            return GL_TRIANGLE_STRIP;
        case PrimitiveType::Lines:
            return GL_LINES;
        case PrimitiveType::LineStrip:
            return GL_LINE_STRIP;
        case PrimitiveType::Points:
            return GL_POINTS;
        default:
            SL_THROW_IF(true, InvalidInputException, "Unknown primitive type")
    }
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


static std::unordered_map<std::string, GLuint> discoverVertexAttributes(GLuint program)
{
    std::unordered_map<std::string, GLuint> attributes;

    GLint activeAttributes = 0;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &activeAttributes);
    if (activeAttributes <= 0)
        return attributes;

    GLint nameMaxLength = 0;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &nameMaxLength);
    if (nameMaxLength <= 0)
        return attributes;

    GLint attribSize;
    GLenum attribType;
    std::vector<GLchar> rawName(nameMaxLength + 1);
    for (GLint i = 0; i < activeAttributes; ++i)
    {
        glGetActiveAttrib(program, i, nameMaxLength, nullptr, &attribSize, &attribType, rawName.data());
        rawName[nameMaxLength] = '\0';
        auto location = glGetAttribLocation(program, rawName.data());
        std::string name = rawName.data();
        attributes[name] = location;
    }

    return attributes;
}


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
    // TODO pass here the value of the handle to eliminate the need to construct the handle
    // before calling this function. Same for other similar functions
    auto rawHandle = handle.empty() ? 0 : indexBuffers.getData(handle.value).rawHandle;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rawHandle);
}


void OpenGLRenderer::bindVertexObject(VertexObjectHandle handle)
{
    auto rawHandle = handle.empty() ? 0 : vertexObjects.getData(handle.value).rawHandle;
    glBindVertexArray(rawHandle);
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
    auto& bufferData = vertexBuffers.getData(handle.value);
    bufferData.rawHandle = rawHandle;
    bufferData.layout = layout;

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
    auto& bufferData = indexBuffers.getData(handle.value);
    bufferData.rawHandle = rawHandle;
    bufferData.elementCount = elementCount;

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


VertexObjectHandle OpenGLRenderer::createVertexObject(const VertexBufferHandle* buffers, int bufferCount, ProgramHandle programHandle)
{
    // TODO validate buffers

    std::unordered_map<std::string, GLuint> attributes;
    if (!programHandle.empty())
    {
        auto rawProgramHandle = programs.getData(programHandle.value).rawHandle;
        attributes = discoverVertexAttributes(rawProgramHandle);
    }

    GLuint rawHandle;
    glGenVertexArrays(1, &rawHandle);
    SL_THROW_IF(rawHandle == 0, InternalException, "Failed to obtain vertex object handle")

    VertexObjectHandle handle;
    handle.value = vertexObjects.reserveHandle();
    vertexObjects.getData(handle.value).rawHandle = rawHandle;

    bindVertexObject(handle);

    auto getAttributeLocation = [&](const char* name, GLint defaultValue)
    {
        auto programAttr = attributes.find(name);
        return programAttr != attributes.end() ? programAttr->second : defaultValue;
    };

    for (auto i = 0; i < bufferCount; i++)
    {
        const auto& bufferHandle = buffers[i];
        const auto& layout = vertexBuffers.getData(bufferHandle.value).layout;
        const auto elementCount = layout.getElementCount();
        int offset = 0;
        for (auto j = 0; i < elementCount; j++)
        {
            auto& el = layout.getElement(j);
            GLint attrLoc = 0;
            switch (el.semantics)
            {
                case VertexBufferLayoutSemantics::Unknown:
                default: break;
                case VertexBufferLayoutSemantics::Position: attrLoc = getAttributeLocation("position", 0); break;
                case VertexBufferLayoutSemantics::Normal: attrLoc = getAttributeLocation("normal", 1); break;
                case VertexBufferLayoutSemantics::Color: attrLoc = getAttributeLocation("color", 2); break;
                case VertexBufferLayoutSemantics::Tangent: attrLoc = getAttributeLocation("tangent", 3); break;
                case VertexBufferLayoutSemantics::Binormal: attrLoc = getAttributeLocation("binormal", 4); break;
                case VertexBufferLayoutSemantics::TexCoord0:
                case VertexBufferLayoutSemantics::TexCoord1:
                case VertexBufferLayoutSemantics::TexCoord2:
                case VertexBufferLayoutSemantics::TexCoord3:
                case VertexBufferLayoutSemantics::TexCoord4:
                case VertexBufferLayoutSemantics::TexCoord5:
                case VertexBufferLayoutSemantics::TexCoord6:
                case VertexBufferLayoutSemantics::TexCoord7:
                    auto idx = static_cast<int>(el.semantics) - static_cast<int>(VertexBufferLayoutSemantics::TexCoord0);
                    auto name = "texCoord" + std::to_string(idx);
                    attrLoc = getAttributeLocation(name.c_str(), 5 + idx);
                    break;
            }

            bindVertexBuffer(bufferHandle);
            const auto stride = layout.getSize();
            glVertexAttribPointer(attrLoc, el.size, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset));
            glEnableVertexAttribArray(attrLoc);
            bindVertexBuffer(EmptyVertexBufferHandle);

            offset += el.size * sizeof(float);
        }
    }

    bindVertexObject(EmptyVertexObjectHandle);

    return handle;
}


void OpenGLRenderer::destroyVertexObject(VertexObjectHandle handle)
{
    SL_THROW_IF(handle.empty(), InvalidInputException, "Vertex object handle is empty")
    auto rawHandle = handle.empty() ? 0 : vertexObjects.getData(handle.value).rawHandle;
    glDeleteVertexArrays(1, &rawHandle);
    vertexObjects.releaseHandle(handle.value);
}


void OpenGLRenderer::renderIndexedVertexObject(PrimitiveType primitiveType, const VertexObjectHandle& vertexObjectHandle, const IndexBufferHandle& indexBufferHandle)
{
    SL_THROW_IF(vertexObjectHandle.empty(), InvalidInputException, "Vertex object handle is empty")
    SL_THROW_IF(indexBufferHandle.empty(), InvalidInputException, "Index buffer handle is empty")

    bindVertexObject(vertexObjectHandle);
    bindIndexBuffer(indexBufferHandle);

    const auto& ibData = indexBuffers.getData(indexBufferHandle.value);
    glDrawElements(convertPrimitiveType(primitiveType), ibData.elementCount, GL_UNSIGNED_SHORT, nullptr);

    bindIndexBuffer(EmptyIndexBufferHandle);
    bindVertexObject(EmptyVertexObjectHandle);
}
