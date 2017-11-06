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

        static auto loadFromFiles(Device *device, const str &vsPath, const str &fsPath) -> sptr<Effect>;
        static auto createFromPrefab(Device *device, EffectPrefab prefab) -> sptr<Effect>;

        virtual ~Effect() {}

    protected:
        Effect() {}
    };
}
