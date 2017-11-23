/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloEffect.h"
#include "SoloOpenGL.h"

namespace solo
{
    namespace gl
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

            static auto createFromPrefab(EffectPrefab prefab) -> sptr<OpenGLEffect>;

            OpenGLEffect(const void *vsSrc, u32 vsSrcLen, const void *fsSrc, u32 fsSrcLen);
            ~OpenGLEffect();

            auto getHandle() const -> GLuint { return handle; }

            auto getUniformInfo(const str &name) -> UniformInfo;
            auto getAttributeInfo(const str &name) -> AttributeInfo;

        private:
            GLuint handle = 0;
            umap<str, UniformInfo> uniforms;
            umap<str, AttributeInfo> attributes;

            void introspectUniforms();
            void introspectAttributes();
        };
    }
}

#endif
