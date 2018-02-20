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
        static auto loadFromSourceFile(Device *device, const str &path) -> sptr<Effect>;
        static auto loadFromDescriptionFile(Device *device, const str &path) -> sptr<Effect>;
        static auto createFromSource(Device *device, const str &source) -> sptr<Effect>;
        static auto createFromDescription(Device *device, const str &description) -> sptr<Effect>;
        static auto createFromShaderSources(
            Device *device,
            const void *vsSrc, u32 vsSrcLen,
            const void *fsSrc, u32 fsSrcLen) -> sptr<Effect>;

        virtual ~Effect() = default;

    protected:
        Effect() = default;
    };
}
