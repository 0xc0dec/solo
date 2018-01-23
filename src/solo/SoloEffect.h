/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

namespace solo
{
    class Device;

    class Effect
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(Effect)

        static auto loadFromFile(Device *device, const str &path) -> sptr<Effect>;
		static auto createFromSource(Device *device, const str &source) -> sptr<Effect>;
        static auto createFromSources(
            Device *device,
            const void *vsSrc, u32 vsSrcLen,
            const void *fsSrc, u32 fsSrcLen) -> sptr<Effect>;

		virtual ~Effect() = default;

    protected:
		Effect() = default;
    };
}
