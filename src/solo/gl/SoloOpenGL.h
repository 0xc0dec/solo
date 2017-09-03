/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include <GL/glew.h>

namespace solo
{
    enum class PrimitiveType;
    enum class FaceCull;
    enum class PolygonMode;
    enum class DepthFunction;
    enum class BlendFactor;
    enum class TextureFormat;
    enum class CubeTextureFace;
    class Vector4;

    namespace gl
    {
        auto toPrimitiveType(PrimitiveType type) -> GLenum;
        auto toInternalTextureFormat(TextureFormat format) -> GLenum;
        auto toTextureFormat(TextureFormat format) -> GLenum;
        auto toBlendFactor(BlendFactor factor) -> GLenum;
        bool findUniformInProgram(GLuint program, const char *name, GLint &location, int32_t &index);
        void applyMinFilter(GLenum target, uint32_t flags);
        void applyMagFilter(GLenum target, uint32_t flags);
        void applyRectWrap(GLenum target, uint32_t flags);
        void applyDepthWrap(uint32_t flags);
        void clear(bool color, bool depth, const Vector4 &clearColor);
        void setViewport(const Vector4 &viewport);
        void setDepthWrite(bool enabled);
        void setDepthTest(bool enabled);
        void setDepthFunction(DepthFunction func);
        void setPolygonMode(PolygonMode mode);
        void setFaceCull(FaceCull cull);
        void setBlend(bool enabled);
        void setBlendFactor(BlendFactor srcFactor, BlendFactor dstFactor);
    }
}

#endif