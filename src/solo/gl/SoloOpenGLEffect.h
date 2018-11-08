/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloEffect.h"
#include "SoloOpenGL.h"

namespace solo
{
    class OpenGLEffect final : public Effect
    {
    public:
        struct UniformInfo
        {
            u32 location;
            u32 samplerIndex;
        };

        struct AttributeInfo
        {
            u32 location;
        };

        OpenGLEffect(const void *vsSrc, u32 vsSrcLen, const void *fsSrc, u32 fsSrcLen);
        ~OpenGLEffect();

        auto handle() const -> GLuint { return handle_; }

        auto uniformInfo(const str &name) -> UniformInfo;
        auto hasAttribute(const str &name) -> bool;
        auto attributeInfo(const str &name) -> AttributeInfo;

    private:
        GLuint handle_ = 0;
        umap<str, UniformInfo> uniforms_;
        umap<str, AttributeInfo> attributes_;

        void introspectUniforms();
        void introspectAttributes();
    };
}

#endif
