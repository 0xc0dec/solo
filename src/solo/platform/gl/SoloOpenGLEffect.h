/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloEffect.h"
#include "SoloOpenGLRenderer.h"

namespace solo
{
    class Device;

    namespace gl
    {
        class Effect final : public solo::Effect
        {
        public:
            static auto create(Device *device, EffectPrefab prefab) -> sptr<Effect>;

            Effect(Device *device, const void *vsSrc, uint32_t vsSrcLen, const void *fsSrc, uint32_t fsSrcLen);
            Effect(Device *device, const std::string &vsSrc, const std::string &fsSrc);
            ~Effect();

            auto getHandle() const -> uint32_t;

            void apply() override final;

        private:
            Renderer *renderer = nullptr;
            uint32_t handle = EmptyHandle;
        };

        inline auto Effect::getHandle() const -> uint32_t
        {
            return handle;
        }
    }
}

#endif
