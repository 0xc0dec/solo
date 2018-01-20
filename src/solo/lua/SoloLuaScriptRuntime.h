/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloScriptRuntime.h"
#include <LuaIntf.h>
#include <sol.hpp>

namespace solo
{
    class LuaScriptRuntime final : public ScriptRuntime
    {
    public:
        LuaScriptRuntime();
        explicit LuaScriptRuntime(Device *device);
        ~LuaScriptRuntime();

        void executeFile(const str& path) override final;

        auto readString(const str &name) -> str override final;
        auto readDeviceSetup(const str &name) -> DeviceSetup override final;

    private:
        LuaIntf::LuaState lua;
    };

	class LuaScriptRuntime2 final: public ScriptRuntime
	{
	public:
		LuaScriptRuntime2();
		explicit LuaScriptRuntime2(Device *device);
		~LuaScriptRuntime2() = default;

		void executeFile(const str& path) override final;

		auto readString(const str &name)->str override final;
		auto readDeviceSetup(const str &name)->DeviceSetup override final;

	private:
		sol::state state;
	};
}