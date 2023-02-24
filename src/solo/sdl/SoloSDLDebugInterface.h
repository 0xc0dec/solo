/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include "SoloDebugInterface.h"

namespace solo {
    class SDLDevice;

    class SDLDebugInterface: public DebugInterface {
    public:
        explicit SDLDebugInterface(Device *device);
        ~SDLDebugInterface();

    protected:
        void beginFrame() override;

    private:
        SDLDevice *device_ = nullptr;
    };
}