/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloEffect.h"
#include "SoloOpenGL.h"
#include <unordered_map>

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

            auto getUniformInfo(const std::string &name) -> UniformInfo;
            auto getAttributeInfo(const std::string &name) -> AttributeInfo;

        private:
            GLuint handle = 0;
            std::unordered_map<std::string, UniformInfo> uniforms;
            std::unordered_map<std::string, AttributeInfo> attributes;

            void introspectUniforms();
            void introspectAttributes();
        };
    }
}

#endif
