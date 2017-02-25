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

        static auto create2(Device *device, const void *vsSrc, uint32_t vsSrcLen, const void *fsSrc, uint32_t fsSrcLen) -> sptr<Effect>;
        // TODO remove
        static auto create(Device *device, const std::string &vsSrc, const std::string &fsSrc) -> sptr<Effect>;
        static auto createFromPrefab(Device *device, EffectPrefab prefab) -> sptr<Effect>;

        virtual ~Effect() {}

        virtual void apply() = 0;

    protected:
        Effect() {}
    };
}
