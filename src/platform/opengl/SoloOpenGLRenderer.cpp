#include "SoloOpenGLRenderer.h"
#include <algorithm>
#include <unordered_map>

using namespace solo;


static GLenum convertCubeTextureFace(CubeTextureFace face)
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
            SL_THROW_FMT(InvalidInputException, "Unknown cube texture face ", static_cast<int>(face));
    }
}


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
            SL_DEBUG_FMT_THROW_IF(true, InvalidInputException, "Unknown primitive type")
    }
}


static GLenum convertColorFormat(ColorFormat format)
{
    switch (format)
    {
        case ColorFormat::RGB:
            return GL_RGB;
        case ColorFormat::RGBA:
            return GL_RGBA;
        default:
            SL_THROW_FMT(InvalidInputException, "Unknown texture format ", static_cast<int>(format));
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
        SL_DEBUG_THROW_IF(true, EffectCompilationException, "Failed to link program", log.data());
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
        SL_DEBUG_THROW_IF(true, EffectCompilationException, SL_FMT("Failed to compile ", typeNames[type], " shader"), log.data());
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
        GLint location = glGetAttribLocation(program, rawName.data());
        std::string name = rawName.data();
        attributes[name] = location;
    }

    return attributes;
}


static bool findUniformInProgram(GLuint program, const char* name, GLint& location, int& index)
{
    GLint activeUniforms;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &activeUniforms);
    if (activeUniforms <= 0)
        return false;
    
    GLint nameMaxLength;
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nameMaxLength);
    if (nameMaxLength <= 0)
        return false;
    
    std::vector<GLchar> rawName(nameMaxLength + 1);
    int samplerIndex = 0;
    for (GLint i = 0; i < activeUniforms; ++i)
    {
        GLint size;
        GLenum type;

        glGetActiveUniform(program, i, nameMaxLength, nullptr, &size, &type, rawName.data());
        rawName[nameMaxLength] = '\0';
        std::string n = rawName.data();
    
        // Strip away possible square brackets for array uniforms,
        // they are sometimes present on some platforms
        auto bracketIndex = n.find('[');
        if (bracketIndex != std::string::npos)
            n.erase(bracketIndex);
    
        int idx = 0;
        if (type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE) // TODO other types of samplers
        {
            idx = samplerIndex;
            samplerIndex += size;
        }

        if (n == name)
        {
            location = glGetUniformLocation(program, rawName.data());
            index = idx;
            return true;
        }
    }

    return false;
}


OpenGLRenderer::OpenGLRenderer(Device* device) :
    Renderer(device)
{
}


OpenGLRenderer::~OpenGLRenderer()
{
    // At this point all resources should already have been released

    // TODO DRY? Never heard of it
    while (programs.getHandleCount() > 0)
    {
        auto handle = programs.getHandle(0);
        auto data = programs.getData(handle);
        glDeleteProgram(data.rawHandle);
        programs.releaseHandle(handle);
    }

    while (vertexObjects.getHandleCount() > 0)
    {
        auto handle = vertexObjects.getHandle(0);
        auto data = vertexObjects.getData(handle);
        glDeleteVertexArrays(1, &data.rawHandle);
        vertexObjects.releaseHandle(handle);
    }

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
    SL_DEBUG_FMT_THROW_IF(rawHandle == 0, InternalException, "Failed to obtain texture handle")

    TextureHandle handle;
    handle.value = textures.reserveHandle();
    textures.getData(handle.value).rawHandle = rawHandle;

    return handle;
}


void OpenGLRenderer::destroyTexture(TextureHandle handle)
{
    SL_DEBUG_FMT_THROW_IF(handle.empty(), InvalidInputException, "Texture handle is empty")
    auto rawHandle = textures.getData(handle.value).rawHandle;
    glDeleteTextures(1, &rawHandle);
    textures.releaseHandle(handle.value);
}


void OpenGLRenderer::update2DTexture(TextureHandle handle, ColorFormat format, int width, int height, const std::vector<uint8_t>& data)
{
    SL_DEBUG_FMT_THROW_IF(width <= 0 || height <= 0, InvalidInputException, "Invalid texture dimensions")
    SL_DEBUG_FMT_THROW_IF(handle.empty(), InvalidInputException, "Texture handle is empty")

    bindTexture(GL_TEXTURE_2D, handle);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        convertColorFormat(format),
        width,
        height,
        0,
        convertColorFormat(format),
        GL_UNSIGNED_BYTE,
        data.size() ? data.data() : 0);

    auto& texData = textures.getData(handle.value);
    texData.width = width;
    texData.height = height;

    bindTexture(GL_TEXTURE_2D, EmptyTextureHandle);
}


void OpenGLRenderer::updateCubeTexture(TextureHandle handle, CubeTextureFace face, ColorFormat format, int width, int height, const std::vector<uint8_t>& data)
{
    SL_DEBUG_FMT_THROW_IF(width <= 0 || height <= 0, InvalidInputException, "Invalid texture dimensions")
    SL_DEBUG_FMT_THROW_IF(handle.empty(), InvalidInputException, "Texture handle is empty")

    bindTexture(GL_TEXTURE_CUBE_MAP, handle);

    auto glFace = convertCubeTextureFace(face);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(
        glFace,
        0,
        convertColorFormat(format),
        width,
        height,
        0,
        convertColorFormat(format),
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
    SL_DEBUG_FMT_THROW_IF(attachments.size() > GL_MAX_COLOR_ATTACHMENTS, InvalidInputException, "Too many frame buffer attachments")

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
    SL_DEBUG_FMT_THROW_IF(rawHandle == 0, InternalException, "Failed to obtain frame buffer handle")

    FrameBufferHandle handle;
    handle.value = frameBuffers.reserveHandle();
    frameBuffers.getData(handle.value).rawHandle = rawHandle;

    return handle;
}


void OpenGLRenderer::destroyFrameBuffer(FrameBufferHandle handle)
{
    SL_DEBUG_FMT_THROW_IF(handle.empty(), InvalidInputException, "Frame buffer handle is empty")
    auto rawHandle = frameBuffers.getData(handle.value).rawHandle;
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
    SL_DEBUG_FMT_THROW_IF(handle.empty(), InvalidInputException, "Frame buffer handle is empty")
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
        SL_DEBUG_FMT_THROW_IF(!data.depthBufferHandle, InternalException, "Failed to obtain depth buffer handle")

        glBindRenderbuffer(GL_RENDERBUFFER, data.depthBufferHandle);
        auto firstAttachmentData = textures.getData(attachments[0].value);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, firstAttachmentData.width, firstAttachmentData.height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, data.depthBufferHandle);

        SL_DEBUG_FMT_THROW_IF(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE,
            InternalException, "Render target has invalid state")
    }

    bindFrameBuffer(EmptyFrameBufferHandle);
}


VertexBufferHandle OpenGLRenderer::createVertexBuffer(const VertexBufferLayout& layout, const void* data, int vertexCount)
{
    GLuint rawHandle = 0;
    glGenBuffers(1, &rawHandle);
    SL_DEBUG_FMT_THROW_IF(rawHandle == 0, InternalException, "Failed to obtain vertex buffer handle")

    VertexBufferHandle handle;
    handle.value = vertexBuffers.reserveHandle();
    auto& bufferData = vertexBuffers.getData(handle.value);
    bufferData.rawHandle = rawHandle;
    bufferData.layout = layout;
    bufferData.vertexCount = vertexCount;

    bindVertexBuffer(handle);
    glBufferData(GL_ARRAY_BUFFER, layout.getSize() * vertexCount, data, GL_STATIC_DRAW);
    bindVertexBuffer(EmptyVertexBufferHandle);

    return handle;
}


void OpenGLRenderer::destroyVertexBuffer(VertexBufferHandle handle)
{
    SL_DEBUG_FMT_THROW_IF(handle.empty(), InvalidInputException, "Vertex buffer handle is empty")
    auto rawHandle = vertexBuffers.getData(handle.value).rawHandle;
    glDeleteBuffers(1, &rawHandle);
    vertexBuffers.releaseHandle(handle.value);
}


IndexBufferHandle OpenGLRenderer::createIndexBuffer(const void* data, int elementSize, int elementCount)
{
    GLuint rawHandle = 0;
    glGenBuffers(1, &rawHandle);
    SL_DEBUG_FMT_THROW_IF(rawHandle == 0, InternalException, "Failed to obtain index buffer handle")

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
    SL_DEBUG_FMT_THROW_IF(handle.empty(), InvalidInputException, "Index buffer handle is empty")
    auto rawHandle = indexBuffers.getData(handle.value).rawHandle;
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
    SL_DEBUG_FMT_THROW_IF(handle.empty(), InvalidInputException, "Program handle is empty")
    auto rawHandle = programs.getData(handle.value).rawHandle;
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
    SL_DEBUG_FMT_THROW_IF(rawHandle == 0, InternalException, "Failed to obtain vertex object handle")

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
        for (auto j = 0; j < elementCount; j++)
        {
            auto& el = layout.getElement(j);
            GLint attrLoc = 0;
            switch (el.semantics)
            {
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
                {
                    auto idx = static_cast<int>(el.semantics) - static_cast<int>(VertexBufferLayoutSemantics::TexCoord0);
                    auto name = "texCoord" + std::to_string(idx);
                    attrLoc = getAttributeLocation(name.c_str(), 5 + idx);
                    break;
                }
                default: break;
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
    SL_DEBUG_FMT_THROW_IF(handle.empty(), InvalidInputException, "Vertex object handle is empty")
    auto rawHandle = vertexObjects.getData(handle.value).rawHandle;
    glDeleteVertexArrays(1, &rawHandle);
    vertexObjects.releaseHandle(handle.value);
}


void OpenGLRenderer::renderIndexedVertexObject(PrimitiveType primitiveType, const VertexObjectHandle& vertexObjectHandle, const IndexBufferHandle& indexBufferHandle)
{
    SL_DEBUG_FMT_THROW_IF(vertexObjectHandle.empty(), InvalidInputException, "Vertex object handle is empty")
    SL_DEBUG_FMT_THROW_IF(indexBufferHandle.empty(), InvalidInputException, "Index buffer handle is empty")

    bindVertexObject(vertexObjectHandle);
    bindIndexBuffer(indexBufferHandle);

    const auto& ibData = indexBuffers.getData(indexBufferHandle.value);
    glDrawElements(convertPrimitiveType(primitiveType), ibData.elementCount, GL_UNSIGNED_SHORT, nullptr);

    bindIndexBuffer(EmptyIndexBufferHandle);
    bindVertexObject(EmptyVertexObjectHandle);
}


void OpenGLRenderer::renderVertexObject(PrimitiveType primitiveType, const VertexObjectHandle& vertexObjectHandle, int vertexCount)
{
    SL_DEBUG_FMT_THROW_IF(vertexObjectHandle.empty(), InvalidInputException, "Vertex object handle is empty")
    bindVertexObject(vertexObjectHandle);
    glDrawArrays(convertPrimitiveType(primitiveType), 0, vertexCount);
    bindVertexObject(EmptyVertexObjectHandle);
}


UniformHandle OpenGLRenderer::createUniform(const char* name, UniformType type, int componentCount, ProgramHandle program)
{
    // TODO needed?
    // TODO so something with error handling. This is a mess
    SL_DEBUG_FMT_THROW_IF(!name, InvalidInputException, "Uniform name is empty")
    SL_DEBUG_FMT_THROW_IF(!componentCount, InvalidInputException, "Uniform element count is zero")
    SL_DEBUG_FMT_THROW_IF(program.empty(), InvalidInputException, "Program handle is empty")

    auto rawProgramHandle = programs.getData(program.value).rawHandle;

    GLint location, index;
    if (!findUniformInProgram(rawProgramHandle, name, location, index))
        return UniformHandle(); // TODO or abort()

    UniformHandle handle;
    handle.value = uniforms.reserveHandle();
    auto& data = uniforms.getData(handle.value);
    data.type = type;
    data.index = index;
    data.location = location;
    data.componentCount = componentCount;

    return handle;
}


void OpenGLRenderer::destroyUniform(const UniformHandle& handle)
{
    programs.releaseHandle(handle.value);
}


void OpenGLRenderer::setUniform(const UniformHandle& handle, const void* value, int count)
{
    const auto& data = uniforms.getData(handle.value);
    auto floatData = reinterpret_cast<const float*>(value);
    switch (data.type)
    {
        case UniformType::Float:
            glUniform1f(data.location, *floatData);
            break;
        case UniformType::FloatArray:
            glUniform1fv(data.location, static_cast<GLsizei>(count), floatData);
            break;
        case UniformType::Vector2:
            glUniform2f(data.location, floatData[0], floatData[1]);
            break;
        case UniformType::Vector2Array:
            glUniform2fv(data.location, static_cast<GLsizei>(count), floatData);
            break;
        case UniformType::Vector3:
            glUniform3f(data.location, floatData[0], floatData[1], floatData[2]);
            break;
        case UniformType::Vector3Array:
            glUniform3fv(data.location, static_cast<GLsizei>(count), floatData);
            break;
        case UniformType::Vector4:
            glUniform4f(data.location, floatData[0], floatData[1], floatData[2], floatData[3]);
            break;
        case UniformType::Vector4Array:
            glUniform4fv(data.location, static_cast<GLsizei>(count), floatData);
            break;
        case UniformType::Matrix:
            glUniformMatrix4fv(data.location, 1, GL_FALSE, floatData);
            break;
        case UniformType::MatrixArray:
            glUniformMatrix4fv(data.location, static_cast<GLsizei>(count), GL_FALSE, floatData);
            break;
        case UniformType::Texture:
            glActiveTexture(GL_TEXTURE0 + data.index);
            glUniform1i(data.location, data.index);
            break;
        case UniformType::TextureArray: break; // TODO
        default: break;
    }
}


void OpenGLRenderer::setState(int stateFlags)
{
    if (stateFlags & CullFace)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);

    glFrontFace(stateFlags & FrontFaceCCW ? GL_CCW : GL_CW);

    if (stateFlags & DepthTest)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    GLenum depthFunc = 0;
    if (stateFlags & DepthFuncAlways)
        depthFunc = GL_ALWAYS;
    else if (stateFlags & DepthFuncEqual)
        depthFunc = GL_EQUAL;
    else if (stateFlags & DepthFuncGEqual)
        depthFunc = GL_GEQUAL;
    else if (stateFlags & DepthFuncGreater)
        depthFunc = GL_GREATER;
    else if (stateFlags & DepthFuncLEqual)
        depthFunc = GL_LEQUAL;
    else if (stateFlags & DepthFuncLess)
        depthFunc = GL_LESS;
    else if (stateFlags & DepthFuncNotEqual)
        depthFunc = GL_NOTEQUAL;
    else
        depthFunc = GL_NEVER;
    
    glDepthFunc(depthFunc);

    glDepthMask(stateFlags & DepthWrite ? GL_TRUE : GL_FALSE);
}


void OpenGLRenderer::setViewport(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
}


void OpenGLRenderer::clear(bool color, bool depth, float r, float g, float b, float a)
{
    if (color)
        glClearColor(r, g, b, a);
    GLbitfield flags = (color ? GL_COLOR_BUFFER_BIT : 0) | (depth ? GL_DEPTH_BUFFER_BIT : 0);
    glClear(flags);
}