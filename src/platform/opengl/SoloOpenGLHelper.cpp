#include "SoloOpenGLHelper.h"
#include "SoloException.h"
#include "SoloImage.h"
#include "SoloTexture.h"
#include "SoloMaterial.h"
#include "SoloMesh.h"

using namespace solo;


GLenum OpenGLHelper::convertToGLColorFormat(ColorFormat format)
{
    switch (format)
    {
    case ColorFormat::RGB:
        return GL_RGB;
    case ColorFormat::RGBA:
        return GL_RGBA;
    default:
        SL_THROW_FMT(EngineException, "Unexpected texture format ", static_cast<int>(format));
    }
}


GLenum OpenGLHelper::convertToGLWrapMode(TextureWrapping mode)
{
    switch (mode)
    {
    case TextureWrapping::Clamp:
        return GL_CLAMP_TO_EDGE;
    case TextureWrapping::Repeat:
        return GL_REPEAT;
    default:
        SL_THROW_FMT(EngineException, "Unexpected wrap mode ", static_cast<int>(mode));
    }
}


GLenum OpenGLHelper::convertToGLFilter(TextureFiltering filter)
{
    switch (filter)
    {
    case TextureFiltering::Linear:
        return GL_LINEAR;
    case TextureFiltering::Nearest:
        return GL_NEAREST;
    case TextureFiltering::LinearMipmapNearest:
        return GL_LINEAR_MIPMAP_NEAREST;
    case TextureFiltering::LinearMipmapLinear:
        return GL_LINEAR_MIPMAP_LINEAR;
    case TextureFiltering::NearestMipmapLinear:
        return GL_NEAREST_MIPMAP_LINEAR;
    case TextureFiltering::NearestMipmapNearest:
        return GL_NEAREST_MIPMAP_NEAREST;
    default:
        SL_THROW_FMT(EngineException, "Unexpected texture filter ", static_cast<int>(filter));
    }
}


GLenum OpenGLHelper::convertToGLDepthFunc(DepthPassFunction func)
{
    switch (func)
    {
    case DepthPassFunction::Never: return GL_NEVER;
    case DepthPassFunction::Less: return GL_LESS;
    case DepthPassFunction::Equal: return GL_EQUAL;
    case DepthPassFunction::LEqual: return GL_LEQUAL;
    case DepthPassFunction::Greater: return GL_GREATER;
    case DepthPassFunction::NotEqual: return GL_NOTEQUAL;
    case DepthPassFunction::GEqual: return GL_GEQUAL;
    case DepthPassFunction::Always: return GL_ALWAYS;
    default:
        SL_THROW_FMT(EngineException, "Unknown depth pass function");
    }
}


GLenum OpenGLHelper::convertMeshPrimitiveType(MeshPrimitiveType type)
{
    switch (type)
    {
    case MeshPrimitiveType::Triangles:
        return GL_TRIANGLES;
    case MeshPrimitiveType::TriangleStrip:
        return GL_TRIANGLE_STRIP;
    case MeshPrimitiveType::Lines:
        return GL_LINES;
    case MeshPrimitiveType::LineStrip:
        return GL_LINE_STRIP;
    case MeshPrimitiveType::Points:
        return GL_POINTS;
    default:
        SL_THROW_FMT(EngineException, "Unknown mesh primitive type");
    }
}


GLenum OpenGLHelper::convertMeshIndexType(MeshIndexFormat indexFormat)
{
    switch (indexFormat)
    {
    case MeshIndexFormat::UnsignedByte:
        return GL_UNSIGNED_BYTE;
    case MeshIndexFormat::UnsignedShort:
        return GL_UNSIGNED_SHORT;
    case MeshIndexFormat::UnsignedInt:
        return GL_UNSIGNED_INT;
    default:
        SL_THROW_FMT(EngineException, "Unknown mesh index type");
    }
}


int OpenGLHelper::getMeshIndexElementSize(MeshIndexFormat indexFormat)
{
    switch (indexFormat)
    {
    case MeshIndexFormat::UnsignedByte:
        return 1;
    case MeshIndexFormat::UnsignedShort:
        return 2;
    case MeshIndexFormat::UnsignedInt:
        return 4;
    default:
        SL_THROW_FMT(EngineException, "Unrecognized index format");
    }
}
