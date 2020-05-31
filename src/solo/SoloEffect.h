/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"

namespace solo
{
    class Device;

    class Effect
    {
    public:
        static auto fromSourceFile(Device *device, const str &path) -> sptr<Effect>;
        static auto fromDescriptionFile(Device *device, const str &path) -> sptr<Effect>;
        static auto fromSource(Device *device, const str &source) -> sptr<Effect>;
        static auto fromDescription(Device *device, const str &description) -> sptr<Effect>;

        Effect(const Effect &other) = delete;
        Effect(Effect &&other) = delete;
    	virtual ~Effect() = default;
    	
        auto operator=(const Effect &other) -> Effect& = delete;
        auto operator=(Effect &&other) -> Effect& = delete;

    protected:
        Effect() = default;
    };
}
