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
        class Effect final : public solo::Effect
        {
        public:
            static auto create(EffectPrefab prefab) -> sptr<Effect>;

            Effect(const void *vsSrc, uint32_t vsSrcLen, const void *fsSrc, uint32_t fsSrcLen);
            ~Effect();

            auto getHandle() const -> GLuint;

        private:
            GLuint handle = 0;
        };

        inline auto Effect::getHandle() const -> GLuint
        {
            return handle;
        }
    }
}

#endif
