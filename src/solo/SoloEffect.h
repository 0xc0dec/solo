/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

namespace solo
{
    class Device;

    enum class EffectPrefab
    {
        Skybox,
        Font
    };

    class Effect
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(Effect)

        static auto create(Device *device, const void *vsSrc, uint32_t vsSrcLen, const void *fsSrc, uint32_t fsSrcLen) -> sptr<Effect>;
        static auto createFromPrefab(Device *device, EffectPrefab prefab) -> sptr<Effect>;

        virtual ~Effect() {}

    protected:
        Effect() {}
    };
}
