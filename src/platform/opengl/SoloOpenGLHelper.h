#pragma once

#include <GL/glew.h>

namespace solo
{
    enum class ColorFormat;
    enum class TextureWrapping;
    enum class TextureFiltering;
    enum class DepthPassFunction;
    enum class MeshIndexFormat;
    enum class MeshPrimitiveType;

    class OpenGLHelper
    {
    public:
        static GLenum convertToGLColorFormat(ColorFormat format);
        static GLenum convertToGLWrapMode(TextureWrapping mode);
        static GLenum convertToGLFilter(TextureFiltering filter);
        static GLenum convertToGLDepthFunc(DepthPassFunction func);
        static GLenum convertMeshPrimitiveType(MeshPrimitiveType type);
        static GLenum convertMeshIndexType(MeshIndexFormat indexFormat);
        static int getMeshIndexElementSize(MeshIndexFormat indexFormat);
    };
}
