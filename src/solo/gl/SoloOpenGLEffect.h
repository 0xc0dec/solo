/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloEffect.h"
#include "SoloOpenGL.h"
#include "SoloMap.h"

namespace solo
{
    namespace gl
    {
        class Effect final : public solo::Effect
        {
        public:
            struct UniformInfo
            {
                uint32_t location;
                uint32_t samplerIndex;
            };

            struct AttributeInfo
            {
                uint32_t location;
            };

            static auto createFromPrefab(EffectPrefab prefab) -> sptr<Effect>;

            Effect(const void *vsSrc, uint32_t vsSrcLen, const void *fsSrc, uint32_t fsSrcLen);
            ~Effect();

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
