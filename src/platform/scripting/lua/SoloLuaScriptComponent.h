#pragma once

#include "SoloComponent.h"
#include <functional>
#include <LuaIntf.h>

namespace solo
{
    class LuaScriptComponent: public ComponentBase<LuaScriptComponent>
    {
    public:
        LuaScriptComponent(const Node& node, LuaIntf::LuaRef component);

        virtual void init() override;
        virtual void update() override;
        virtual void terminate() override;
        virtual void render(RenderContext& context) override;
        virtual void onAfterCameraRender() override;

        virtual int getTypeId() override;

        static Component* findComponent(Node* node, const std::string& name);
        static Component* addComponent(Node* node, const std::string& name);
        static void removeComponent(Node* node, const std::string& name);

        static std::function<LuaIntf::LuaRef(Node*, const std::string&)> getFindScriptFunc(lua_State* lua);
        static void addScript(Node* node, LuaIntf::LuaRef component);
        static void removeScript(Node* node, const std::string& name);

    private:
        static LuaIntf::LuaRef findScript(lua_State* lua, Node* node, const std::string& name);

        static int getOrRegisterComponentTypeId(const std::string& name);
        static int findComponentTypeId(const std::string& name);

        static int counter;
        static std::unordered_map<std::string, int> typeIds;

        int typeId;
        LuaIntf::LuaRef component;
        std::function<void(LuaIntf::LuaRef)> initFunc;
        std::function<void(LuaIntf::LuaRef)> updateFunc;
        std::function<void(LuaIntf::LuaRef)> terminateFunc;
        std::function<void(LuaIntf::LuaRef, RenderContext&)> renderFunc;
        std::function<void(LuaIntf::LuaRef)> onAfterCameraRenderFunc;
    };
}