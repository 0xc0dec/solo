#pragma once

#include "SoloBase.h"


namespace solo
{
    class Device;

    class ScriptManager
    {
    public:
        static shared<ScriptManager> create(Device* device);

        SL_NONCOPYABLE(ScriptManager)
        virtual ~ScriptManager() {}

        virtual void execute(const std::string& code) = 0;
        virtual void executeFile(const std::string& path) = 0;

    protected:
        explicit ScriptManager(Device* device);

        Device* device = nullptr;
    };
}
