#pragma once

#include "SoloBase.h"


namespace solo
{
    class ScriptManager
    {
    public:
        static shared<ScriptManager> create();

        SL_NONCOPYABLE(ScriptManager)
        virtual ~ScriptManager() {}

        virtual void execute(const std::string& code) = 0;
        virtual void executeFile(const std::string& path) = 0;

    protected:
        ScriptManager() {}
    };
}
