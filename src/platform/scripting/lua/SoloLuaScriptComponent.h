#pragma once

#include "SoloComponent.h"
#include <functional>
#include <LuaIntf.h>

namespace solo
{
    class LuaScriptComponent: public ComponentBase<LuaScriptComponent>
    {
    public:
        LuaScriptComponent(const Node& node, LuaIntf::LuaRef scriptComponent);

        virtual void init() override;
        virtual void update() override;
        virtual void terminate() override;
        virtual void render(RenderContext& context) override;
        virtual void onAfterCameraRender() override;

        virtual uint32_t getTypeId() override;

        static Component* findComponent(Node* node, const std::string& name);
        static Component* addComponent(Node* node, const std::string& name);
        static void removeComponent(Node* node, const std::string& name);

        static void addScript(Node* node, LuaIntf::LuaRef scriptComponent);
        static void removeScript(Node* node, LuaIntf::LuaRef scriptComponent);

    private:
        uint32_t typeId;
        LuaIntf::LuaRef scriptComponent;
        std::function<void(LuaIntf::LuaRef)> initFunc;
        std::function<void(LuaIntf::LuaRef)> updateFunc;
        std::function<void(LuaIntf::LuaRef)> terminateFunc;
        std::function<void(LuaIntf::LuaRef, RenderContext&)> renderFunc;
        std::function<void(LuaIntf::LuaRef)> onAfterCameraRenderFunc;
    };
}