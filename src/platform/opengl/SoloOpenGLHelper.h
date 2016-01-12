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
    enum class CubeTextureFace;

    class OpenGLHelper
    {
    public:
        static GLenum convertColorFormat(ColorFormat format);
        static GLenum convertTextureWrapping(TextureWrapping mode);
        static GLenum convertTextureFiltering(TextureFiltering filter);
        static GLenum convertDepthPassFunction(DepthPassFunction func);
        static GLenum convertMeshPrimitiveType(MeshPrimitiveType type);
        static GLenum convertMeshIndexType(MeshIndexFormat indexFormat);
        static GLenum convertCubeTextureFace(CubeTextureFace face);
        static int getMeshIndexElementSize(MeshIndexFormat indexFormat);
    };
}
