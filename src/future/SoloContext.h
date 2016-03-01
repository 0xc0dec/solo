#pragma once

#include "SoloBase.h"

namespace solo
{
    enum class DeviceMode;
    class RenderContext;

    class Context
    {
    public:
        explicit Context(bool multithreaded);
        ~Context();

        void waitFrame();
        void swap();

        SL_NONCOPYABLE(Context);

    private:
        bool multithreaded;
        shared<RenderContext> renderContext;
    };
}