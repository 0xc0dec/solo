/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

namespace solo
{
    class Device;

    class Effect: public NoCopyAndMove
    {
    public:
        static auto fromSourceFile(Device *device, const str &path) -> sptr<Effect>;
        static auto fromDescriptionFile(Device *device, const str &path) -> sptr<Effect>;
        static auto fromSource(Device *device, const str &source) -> sptr<Effect>;
        static auto fromDescription(Device *device, const str &description) -> sptr<Effect>;
        static auto fromShaderSources(
            Device *device,
            const void *vsSrc, u32 vsSrcLen,
            const void *fsSrc, u32 fsSrcLen) -> sptr<Effect>;

        virtual ~Effect() = default;

    protected:
        Effect() = default;
    };
}
