#pragma once

#include "SoloScriptManager.h"
#include <LuaIntf.h>


namespace solo
{
    class LuaScriptManager final: public ScriptManager
    {
    public:
        virtual ~LuaScriptManager();

        virtual void execute(const std::string& code) override final;
        virtual void executeFile(const std::string& path) override final;

    private:
        friend class ScriptManager;

        LuaScriptManager(Device* device);

        void registerApi();

        LuaIntf::LuaState lua;
    };
}