/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#include "SoloOpenGLRenderer.h"
#include "SoloDevice.h"
#include "SoloLogger.h"
#include <algorithm>
#include <unordered_map>

#ifdef SL_OPENGL_RENDERER

using namespace solo;


static auto toGLBlendFactor(BlendFactor factor) -> GLenum
{
    switch (factor)
    {
        case BlendFactor::Zero: return GL_ZERO;
        case BlendFactor::One: return GL_ONE;
        case BlendFactor::SrcColor: return GL_SRC_COLOR;
        case BlendFactor::OneMinusSrcColor: return GL_ONE_MINUS_SRC_COLOR;
        case BlendFactor::DstColor: return GL_DST_COLOR;
        case BlendFactor::OneMinusDstColor: return GL_ONE_MINUS_DST_COLOR;
        case BlendFactor::SrcAlpha: return GL_SRC_ALPHA;
        case BlendFactor::OneMinusSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;
        case BlendFactor::DstAlpha: return GL_DST_ALPHA;
        case BlendFactor::OneMinusDstAlpha: return GL_ONE_MINUS_DST_ALPHA;
        case BlendFactor::ConstantAlpha: return GL_CONSTANT_ALPHA;
        case BlendFactor::OneMinusConstantAlpha: return GL_ONE_MINUS_CONSTANT_ALPHA;
        case BlendFactor::SrcAlphaSaturate: return GL_SRC_ALPHA_SATURATE;
        default:
            SL_ERR("Unknown blend factor");
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
            SL_ERR("Unknown cube texture face");
            return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
    }
}


static auto toGLPrimitiveType(PrimitiveType type) -> GLenum
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
            SL_ERR("Unknown primitive type");
            return GL_TRIANGLES;
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
            SL_ERR("Unknown texture format");
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
            SL_ERR("Unknown texture format");
            return GL_RGB;
    }
}


static auto linkProgram(GLuint vs, GLuint fs) -> GLint
{
    auto program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> log(logLength);
        glGetProgramInfoLog(program, logLength, nullptr, log.data());
        glDeleteProgram(program);
        SL_ERR("Failed to link program", log.data());
    }

    return program;
}


static auto compileShader(GLuint type, const char* src) -> GLint
{
    static std::unordered_map<GLuint, std::string> typeNames =
    {
        { GL_VERTEX_SHADER, "vertex" },
        { GL_FRAGMENT_SHADER, "fragment" }
    };

    auto shader = glCreateShader(type);

    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> log(logLength);
        glGetShaderInfoLog(shader, logLength, nullptr, log.data());
        glDeleteShader(shader);
        SL_ERR(SL_FMT("Failed to compile ", typeNames[type], " shader"), log.data());
    }

    return shader;
}


static auto discoverVertexAttributes(GLuint program) -> std::unordered_map<std::string, GLuint>
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


static bool findUniformInProgram(GLuint program, const char* name, GLint& location, int32_t& index)
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
    uint32_t samplerIndex = 0;
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
    
        uint32_t idx = 0;
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


OpenGLRenderer::OpenGLRenderer(Device* device)
{
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    device->getLogger()->logInfo(SL_FMT("Running in OpenGL ", major, ".", minor, " mode"));
}


OpenGLRenderer::~OpenGLRenderer()
{
    // All resources at this point should have already been released
    programs.cleanup([](const Program& program) { glDeleteProgram(program.rawHandle); });
    vertexProgramBindings.cleanup([](const VertexProgramBinding& vo) { glDeleteVertexArrays(1, &vo.rawHandle); });
    frameBuffers.cleanup([](const FrameBuffer& buf) { glDeleteFramebuffers(1, &buf.rawHandle); });
    vertexBuffers.cleanup([](const VertexBuffer& buf) { glDeleteBuffers(1, &buf.rawHandle); });
    indexBuffers.cleanup([](const IndexBuffer& buf) { glDeleteBuffers(1, &buf.rawHandle); });
    textures.cleanup([](const Texture& tex) { glDeleteTextures(1, &tex.rawHandle); });
}


auto OpenGLRenderer::createTexture() -> TextureHandle
{
    GLuint rawHandle = 0;
    glGenTextures(1, &rawHandle);
    SL_ERR_IF(!rawHandle, "Failed to obtain texture handle");

    TextureHandle handle;
    handle.value = textures.reserveHandle();
    textures.getData(handle.value).rawHandle = rawHandle;

    return handle;
}


void OpenGLRenderer::destroyTexture(const TextureHandle& handle)
{
    auto rawHandle = textures.getData(handle.value).rawHandle;
    glDeleteTextures(1, &rawHandle);
    textures.releaseHandle(handle.value);
}


void OpenGLRenderer::update2DTexture(const TextureHandle& handle, TextureFormat format, uint32_t width, uint32_t height, const void* data)
{
    bindTexture(GL_TEXTURE_2D, handle);

    auto internalFormat = toInternalTextureFormat(format);
    auto fmt = toTextureFormat(format);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, fmt, GL_UNSIGNED_BYTE, data);

    auto& texture = textures.getData(handle.value);
    texture.width = width;
    texture.height = height;

    bindTexture(GL_TEXTURE_2D, EmptyTextureHandle);
}


void OpenGLRenderer::updateCubeTexture(const TextureHandle& handle, CubeTextureFace face, TextureFormat format, uint32_t width, uint32_t height, const void* data)
{
    bindTexture(GL_TEXTURE_CUBE_MAP, handle);

    auto glFace = toGLCubeTextureFace(face);
    auto internalFormat = toInternalTextureFormat(format);
    auto fmt = toTextureFormat(format);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(glFace, 0, internalFormat, width, height, 0, fmt, GL_UNSIGNED_BYTE, data);

    // NB width and height in texture data are not updated intentionally

    bindTexture(GL_TEXTURE_CUBE_MAP, EmptyTextureHandle);
}


void OpenGLRenderer::generateRectTextureMipmaps(const TextureHandle& handle)
{
    bindTexture(GL_TEXTURE_2D, handle);
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glGenerateMipmap(GL_TEXTURE_2D);
    bindTexture(GL_TEXTURE_2D, EmptyTextureHandle);
}


void OpenGLRenderer::generateCubeTextureMipmaps(const TextureHandle& handle)
{
    bindTexture(GL_TEXTURE_CUBE_MAP, handle);
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    bindTexture(GL_TEXTURE_CUBE_MAP, EmptyTextureHandle);
}


void OpenGLRenderer::bindTexture(GLenum target, const TextureHandle& handle)
{
    auto rawHandle = handle.empty() ? 0 : textures.getData(handle.value).rawHandle;
    glBindTexture(target, rawHandle);
}


void OpenGLRenderer::bindVertexBuffer(const VertexBufferHandle& handle)
{
    auto rawHandle = handle.empty() ? 0 : vertexBuffers.getData(handle.value).rawHandle;
    glBindBuffer(GL_ARRAY_BUFFER, rawHandle);
}


void OpenGLRenderer::bindIndexBuffer(const IndexBufferHandle& handle)
{
    auto rawHandle = handle.empty() ? 0 : indexBuffers.getData(handle.value).rawHandle;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rawHandle);
}


void OpenGLRenderer::bindVertexProgramBinding(const VertexProgramBindingHandle& handle)
{
    auto rawHandle = handle.empty() ? 0 : vertexProgramBindings.getData(handle.value).rawHandle;
    glBindVertexArray(rawHandle);
}


void OpenGLRenderer::setTexture(GLenum target, const TextureHandle& handle, uint32_t flags)
{
    bindTexture(target, handle);

    if (!flags || handle.empty())
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


void OpenGLRenderer::validateFrameBufferAttachments(const std::vector<TextureHandle>& attachments)
{
    SL_ERR_IF(attachments.size() > GL_MAX_COLOR_ATTACHMENTS, "Too many attachments");

    auto width = -1, height = -1;
    for (auto i = 0; i < attachments.size(); i++)
    {
        auto texture = textures.getData(attachments[i].value);
        if (width < 0)
        {
            width = texture.width;
            height = texture.height;
        }
        else
            SL_ERR_IF(texture.width != width || texture.height != height, "Attachment sizes do not match")
    }
}


void OpenGLRenderer::set2DTexture(const TextureHandle& handle)
{
    bindTexture(GL_TEXTURE_2D, handle);
}


void OpenGLRenderer::set2DTexture(const TextureHandle& handle, uint32_t flags)
{
    setTexture(GL_TEXTURE_2D, handle, flags);
}


void OpenGLRenderer::set2DTexture(const TextureHandle& handle, uint32_t flags, float anisotropyLevel)
{
    set2DTexture(handle, flags);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropyLevel);
}


void OpenGLRenderer::setCubeTexture(const TextureHandle& handle)
{
    bindTexture(GL_TEXTURE_CUBE_MAP, handle);
}


void OpenGLRenderer::setCubeTexture(const TextureHandle& handle, uint32_t flags)
{
    setTexture(GL_TEXTURE_CUBE_MAP, handle, flags);

    if (handle.empty())
        return;

    GLenum wrapR = 0;
    if (flags & TextureFlags::DepthWrapClamp)
        wrapR = GL_CLAMP_TO_EDGE;
    else if (flags & TextureFlags::DepthWrapRepeat)
        wrapR = GL_REPEAT;
    if (wrapR)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapR);
}


void OpenGLRenderer::setCubeTexture(const TextureHandle& handle, uint32_t flags, float anisotropyLevel)
{
    setCubeTexture(handle, flags);
    if (!handle.empty())
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropyLevel);
}


void OpenGLRenderer::bindFrameBuffer(const FrameBufferHandle& handle)
{
    auto rawHandle = handle.empty() ? 0 : frameBuffers.getData(handle.value).rawHandle;
    glBindFramebuffer(GL_FRAMEBUFFER, rawHandle);
}


auto OpenGLRenderer::createFrameBuffer() -> FrameBufferHandle
{
    GLuint rawHandle = 0;
    glGenFramebuffers(1, &rawHandle);
    SL_ERR_IF(!rawHandle, "Failed to obtain frame buffer handle");

    FrameBufferHandle handle;
    handle.value = frameBuffers.reserveHandle();
    frameBuffers.getData(handle.value).rawHandle = rawHandle;

    return handle;
}


void OpenGLRenderer::destroyFrameBuffer(const FrameBufferHandle& handle)
{
    auto rawHandle = frameBuffers.getData(handle.value).rawHandle;
    glDeleteFramebuffers(1, &rawHandle);
    // TODO release depth buffer?
    frameBuffers.releaseHandle(handle.value);
}


void OpenGLRenderer::setFrameBuffer(const FrameBufferHandle& handle)
{
    bindFrameBuffer(handle);
}


void OpenGLRenderer::updateFrameBuffer(const FrameBufferHandle& handle, const std::vector<TextureHandle>& attachmentHandles)
{
    SL_DBG_BLOCK(validateFrameBufferAttachments(attachmentHandles));

    bindFrameBuffer(handle);

    auto frameBuffer = frameBuffers.getData(handle.value);

    if (frameBuffer.depthBufferHandle)
    {
        glDeleteRenderbuffers(1, &frameBuffer.depthBufferHandle);
        frameBuffer.depthBufferHandle = 0;
    }

    auto newCount = attachmentHandles.size();
    auto maxCount = std::max(newCount, static_cast<size_t>(frameBuffer.attachmentCount));
    for (auto i = 0; i < maxCount; i++)
    {
        auto rawHandle = i < newCount ? textures.getData(attachmentHandles[i].value).rawHandle : 0;
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, rawHandle, 0);
    }

    if (newCount > 0)
    {
        // Re-create the depth buffer
        glGenRenderbuffers(1, &frameBuffer.depthBufferHandle);
        SL_ERR_IF(!frameBuffer.depthBufferHandle, "Failed to obtain depth buffer handle");

        glBindRenderbuffer(GL_RENDERBUFFER, frameBuffer.depthBufferHandle);
        auto firstAttachment = textures.getData(attachmentHandles[0].value);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, firstAttachment.width, firstAttachment.height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, frameBuffer.depthBufferHandle);

        SL_ERR_IF(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE, "Render target has invalid state");
    }

    bindFrameBuffer(EmptyFrameBufferHandle);
}


auto OpenGLRenderer::createVertexBuffer(bool dynamic, const VertexBufferLayout& layout, const void* data, uint32_t vertexCount) -> VertexBufferHandle
{
    GLuint rawHandle = 0;
    glGenBuffers(1, &rawHandle);
    SL_ERR_IF(!rawHandle, "Failed to obtain vertex buffer handle");

    glBindBuffer(GL_ARRAY_BUFFER, rawHandle);
    glBufferData(GL_ARRAY_BUFFER, layout.getSize() * vertexCount, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    VertexBufferHandle handle;
    handle.value = vertexBuffers.reserveHandle();
    auto& vertexBuffer = vertexBuffers.getData(handle.value);
    vertexBuffer.rawHandle = rawHandle;
    vertexBuffer.layout = layout;
    vertexBuffer.vertexCount = vertexCount;
    vertexBuffer.dynamic = dynamic;

    return handle;
}


auto OpenGLRenderer::createVertexBuffer(const VertexBufferLayout& layout, const void* data, uint32_t vertexCount) -> VertexBufferHandle
{
    return createVertexBuffer(false, layout, data, vertexCount);
}


auto OpenGLRenderer::createDynamicVertexBuffer(const VertexBufferLayout& layout, const void* data, uint32_t vertexCount) -> VertexBufferHandle
{
    return createVertexBuffer(true, layout, data, vertexCount);
}


void OpenGLRenderer::updateDynamicVertexBuffer(const VertexBufferHandle& handle, const void* data, uint32_t offset, uint32_t vertexCount)
{
    bindVertexBuffer(handle);
    glBufferSubData(GL_ARRAY_BUFFER, offset, vertexCount, data);
    bindVertexBuffer(EmptyVertexBufferHandle);
}


void OpenGLRenderer::destroyVertexBuffer(const VertexBufferHandle& handle)
{
    auto rawHandle = vertexBuffers.getData(handle.value).rawHandle;
    glDeleteBuffers(1, &rawHandle);
    vertexBuffers.releaseHandle(handle.value);
}


auto OpenGLRenderer::createIndexBuffer(const void* data, uint32_t elementSize, uint32_t elementCount) -> IndexBufferHandle
{
    GLuint rawHandle = 0;
    glGenBuffers(1, &rawHandle);
    SL_ERR_IF(!rawHandle, "Failed to obtain index buffer handle");

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rawHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementSize * elementCount, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    IndexBufferHandle handle;
    handle.value = indexBuffers.reserveHandle();
    auto& indexBuffer = indexBuffers.getData(handle.value);
    indexBuffer.rawHandle = rawHandle;
    indexBuffer.elementCount = elementCount;

    return handle;
}


void OpenGLRenderer::destroyIndexBuffer(const IndexBufferHandle& handle)
{
    auto rawHandle = indexBuffers.getData(handle.value).rawHandle;
    glDeleteBuffers(1, &rawHandle);
    indexBuffers.releaseHandle(handle.value);
}


auto OpenGLRenderer::createProgram(const char* vsSrc, const char* fsSrc) -> ProgramHandle
{
    auto vs = compileShader(GL_VERTEX_SHADER, vsSrc);
    auto fs = compileShader(GL_FRAGMENT_SHADER, fsSrc);
    auto program = ::linkProgram(vs, fs);

    glDetachShader(program, vs);
    glDeleteShader(vs);
    glDetachShader(program, fs);
    glDeleteShader(fs);

    ProgramHandle handle;
    handle.value = programs.reserveHandle();
    programs.getData(handle.value).rawHandle = program;

    return handle;
}


void OpenGLRenderer::destroyProgram(const ProgramHandle& handle)
{
    auto rawHandle = programs.getData(handle.value).rawHandle;
    glDeleteProgram(rawHandle);
    programs.releaseHandle(handle.value);
}


void OpenGLRenderer::setProgram(const ProgramHandle& handle)
{
    auto rawHandle = handle.empty() ? 0 : programs.getData(handle.value).rawHandle;
    glUseProgram(rawHandle);
}


auto OpenGLRenderer::createVertexProgramBinding(const VertexBufferHandle* bufferHandles, uint32_t bufferCount, ProgramHandle programHandle)
    -> VertexProgramBindingHandle
{
    GLuint rawHandle;
    glGenVertexArrays(1, &rawHandle);
    SL_ERR_IF(!rawHandle, "Failed to obtain vertex object handle");

    glBindVertexArray(rawHandle);

    auto attributes = discoverVertexAttributes(programs.getData(programHandle.value).rawHandle);

    auto findAttributeLocation = [&](const char* name)
    {
        auto programAttr = attributes.find(name);
        return programAttr != attributes.end() ? programAttr->second : -1;
    };

    for (uint32_t i = 0; i < bufferCount; i++)
    {
        const auto& bufferHandle = bufferHandles[i];
        const auto& layout = vertexBuffers.getData(bufferHandle.value).layout;
        const auto elementCount = layout.getElementCount();
        if (!elementCount)
            continue;

        bindVertexBuffer(bufferHandle);

        // TODO investigate this - remove by-name lookup and rely on layout locations only

        uint32_t offset = 0;
        for (uint32_t j = 0; j < elementCount; j++)
        {
            auto& el = layout.getElement(j);
            auto attrLoc = 0;
            switch (el.semantics)
            {
                case VertexBufferLayoutSemantics::Position: attrLoc = findAttributeLocation("position"); break;
                case VertexBufferLayoutSemantics::Normal: attrLoc = findAttributeLocation("normal"); break;
                case VertexBufferLayoutSemantics::Color: attrLoc = findAttributeLocation("color"); break;
                case VertexBufferLayoutSemantics::Tangent: attrLoc = findAttributeLocation("tangent"); break;
                case VertexBufferLayoutSemantics::Binormal: attrLoc = findAttributeLocation("binormal"); break;
                case VertexBufferLayoutSemantics::TexCoord0:
                case VertexBufferLayoutSemantics::TexCoord1:
                case VertexBufferLayoutSemantics::TexCoord2:
                case VertexBufferLayoutSemantics::TexCoord3:
                case VertexBufferLayoutSemantics::TexCoord4:
                case VertexBufferLayoutSemantics::TexCoord5:
                case VertexBufferLayoutSemantics::TexCoord6:
                case VertexBufferLayoutSemantics::TexCoord7:
                {
                    auto idx = static_cast<uint32_t>(el.semantics) - static_cast<uint32_t>(VertexBufferLayoutSemantics::TexCoord0);
                    auto name = "texCoord" + std::to_string(idx);
                    attrLoc = findAttributeLocation(name.c_str());
                    break;
                }
                default: break;
            }

            if (attrLoc >= 0)
            {
                const auto stride = layout.getSize();
                glVertexAttribPointer(attrLoc, el.size, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset));
                glEnableVertexAttribArray(attrLoc);
            }

            offset += el.size * sizeof(float);
        }

        bindVertexBuffer(EmptyVertexBufferHandle);
    }

    glBindVertexArray(0);

    VertexProgramBindingHandle handle;
    handle.value = vertexProgramBindings.reserveHandle();
    vertexProgramBindings.getData(handle.value).rawHandle = rawHandle;

    return handle;
}


void OpenGLRenderer::destroyVertexProgramBinding(const VertexProgramBindingHandle& handle)
{
    auto rawHandle = vertexProgramBindings.getData(handle.value).rawHandle;
    glDeleteVertexArrays(1, &rawHandle);
    vertexProgramBindings.releaseHandle(handle.value);
}


void OpenGLRenderer::drawIndexed(PrimitiveType primitiveType, const VertexProgramBindingHandle& bindingHandle,
    const IndexBufferHandle& indexBufferHandle)
{
    bindVertexProgramBinding(bindingHandle);
    bindIndexBuffer(indexBufferHandle);

    const auto& indexBuffer = indexBuffers.getData(indexBufferHandle.value);
    glDrawElements(toGLPrimitiveType(primitiveType), indexBuffer.elementCount, GL_UNSIGNED_SHORT, nullptr);

    bindIndexBuffer(EmptyIndexBufferHandle);
    bindVertexProgramBinding(EmptyVertexProgramBindingHandle);
}


void OpenGLRenderer::draw(PrimitiveType primitiveType, const VertexProgramBindingHandle& bindingHandle, uint32_t vertexCount)
{
    bindVertexProgramBinding(bindingHandle);
    glDrawArrays(toGLPrimitiveType(primitiveType), 0, vertexCount);
    bindVertexProgramBinding(EmptyVertexProgramBindingHandle);
}


auto OpenGLRenderer::createUniform(const char* name, UniformType type, ProgramHandle programHandle) -> UniformHandle
{
    auto rawProgramHandle = programs.getData(programHandle.value).rawHandle;

    GLint location, index;
    auto found = findUniformInProgram(rawProgramHandle, name, location, index);
    SL_ERR_IF(!found, SL_FMT("Could not find uniform '", name, "'"));

    UniformHandle handle;
    handle.value = uniforms.reserveHandle();
    auto& uniform = uniforms.getData(handle.value);
    uniform.type = type;
    uniform.index = index;
    uniform.location = location;

    return handle;
}


void OpenGLRenderer::destroyUniform(const UniformHandle& handle)
{
    uniforms.releaseHandle(handle.value);
}


void OpenGLRenderer::setUniform(const UniformHandle& handle, const void* value, uint32_t count)
{
    const auto& uniform = uniforms.getData(handle.value);
    auto floatData = reinterpret_cast<const float*>(value);
    switch (uniform.type)
    {
        case UniformType::Float:
            glUniform1f(uniform.location, *floatData);
            break;
        case UniformType::FloatArray:
            glUniform1fv(uniform.location, static_cast<GLsizei>(count), floatData);
            break;
        case UniformType::Vector2:
            glUniform2f(uniform.location, floatData[0], floatData[1]);
            break;
        case UniformType::Vector2Array:
            glUniform2fv(uniform.location, static_cast<GLsizei>(count), floatData);
            break;
        case UniformType::Vector3:
            glUniform3f(uniform.location, floatData[0], floatData[1], floatData[2]);
            break;
        case UniformType::Vector3Array:
            glUniform3fv(uniform.location, static_cast<GLsizei>(count), floatData);
            break;
        case UniformType::Vector4:
            glUniform4f(uniform.location, floatData[0], floatData[1], floatData[2], floatData[3]);
            break;
        case UniformType::Vector4Array:
            glUniform4fv(uniform.location, static_cast<GLsizei>(count), floatData);
            break;
        case UniformType::Matrix:
            glUniformMatrix4fv(uniform.location, 1, GL_FALSE, floatData);
            break;
        case UniformType::MatrixArray:
            glUniformMatrix4fv(uniform.location, static_cast<GLsizei>(count), GL_FALSE, floatData);
            break;
        case UniformType::Texture:
            glActiveTexture(GL_TEXTURE0 + uniform.index);
            glUniform1i(uniform.location, uniform.index);
            break;
        case UniformType::TextureArray: break; // TODO
        default: break;
    }
}


void OpenGLRenderer::setDepthWrite(bool enabled)
{
    enabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}


void OpenGLRenderer::setDepthTest(bool enabled)
{
    glDepthMask(enabled ? GL_TRUE : GL_FALSE);
}


void OpenGLRenderer::setDepthFunction(DepthFunction func)
{
    GLenum glfunc = 0;
    switch (func)
    {
        case DepthFunction::Never: glfunc = GL_NEVER; break;
        case DepthFunction::Less: glfunc = GL_LESS; break;
        case DepthFunction::Equal: glfunc = GL_EQUAL; break;
        case DepthFunction::LEqual: glfunc = GL_LEQUAL; break;
        case DepthFunction::Greater: glfunc = GL_GREATER; break;
        case DepthFunction::NotEqual: glfunc = GL_NOTEQUAL; break;
        case DepthFunction::GEqual: glfunc = GL_GEQUAL; break;
        case DepthFunction::Always: glfunc = GL_ALWAYS; break;
        default: break;
    }
    if (glfunc)
        glDepthFunc(glfunc);
}


void OpenGLRenderer::setBlend(bool enabled)
{
    enabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
}


void OpenGLRenderer::setBlendFactor(BlendFactor srcFactor, BlendFactor dstFactor)
{
    glBlendFunc(toGLBlendFactor(srcFactor), toGLBlendFactor(dstFactor));
}


void OpenGLRenderer::setFaceCull(FaceCull cull)
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


void OpenGLRenderer::setPolygonMode(PolygonMode mode)
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
        default: return;
    }

    glPolygonMode(GL_FRONT_AND_BACK, glMode);
}


void OpenGLRenderer::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
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

#else
#   error OpenGL renderer is not supported on this platform
#endif
