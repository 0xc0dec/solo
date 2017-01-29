/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#include "SoloLuaScriptComponent.h"
#include "SoloRenderContext.h"
#include "SoloTransform.h"
#include "SoloMeshRenderer.h"
#include "SoloSkyboxRenderer.h"
#include "SoloFontRenderer.h"
#include "SoloCamera.h"
#include "SoloSpectator.h"
#include "SoloLuaCommon.h"
#include "SoloRigidBody.h"

using namespace solo;


static auto findComponent(Node *node, const std::string &name) -> Component*
{
    if (name == "Transform")
        return node->findComponent<Transform>();
    if (name == "MeshRenderer")
        return node->findComponent<MeshRenderer>();
    if (name == "Camera")
        return node->findComponent<Camera>();
    if (name == "Spectator")
        return node->findComponent<Spectator>();
    if (name == "SkyboxRenderer")
        return node->findComponent<SkyboxRenderer>();
    if (name == "FontRenderer")
        return node->findComponent<FontRenderer>();
    if (name == "RigidBody")
        return node->findComponent<RigidBody>();

    SL_ERR("Unknown standard component ", name)
    return nullptr;
}


static auto addComponent(Node *node, const std::string &name, LuaRef arg) -> Component*
{
    if (name == "Transform")
        return node->addComponent<Transform>();
    if (name == "MeshRenderer")
        return node->addComponent<MeshRenderer>();
    if (name == "Camera")
        return node->addComponent<Camera>();
    if (name == "Spectator")
        return node->addComponent<Spectator>();
    if (name == "SkyboxRenderer")
        return node->addComponent<SkyboxRenderer>();
    if (name == "FontRenderer")
        return node->addComponent<FontRenderer>();
    if (name == "RigidBody")
        return node->addComponent<RigidBody>(arg.toValue<RigidBodyConstructionParameters>());

    SL_ERR("Unknown standard component ", name)
    return nullptr;
}


static void removeComponent(Node *node, const std::string &name)
{
    if (name == "Transform")
        node->removeComponent<Transform>();
    else if (name == "MeshRenderer")
        node->removeComponent<MeshRenderer>();
    else if (name == "Camera")
        node->removeComponent<Camera>();
    else if (name == "Spectator")
        node->removeComponent<Spectator>();
    else if (name == "SkyboxRenderer")
        node->removeComponent<SkyboxRenderer>();
    else if (name == "FontRenderer")
        node->removeComponent<FontRenderer>();
    else if (name == "RigidBody")
        node->removeComponent<RigidBody>();
    else
        SL_ERR("Unknown standard component ", name)
}


static void addScriptComponent(Node *node, LuaRef scriptComponent)
{
    auto actualComponent = std::make_shared<LuaScriptComponent>(*node, scriptComponent);
    node->getScene()->addComponent(node->getId(), actualComponent);
}


static void removeScriptComponent(Node *node, LuaRef scriptComponent)
{
    auto typeId = scriptComponent.get<uint32_t>("typeId") + LuaScriptComponent::MinComponentTypeId;
    node->getScene()->removeComponent(node->getId(), typeId);
}


static int lala(Node *node, LuaRef smth)
{
    return smth.toValue<Transform*>()->getTags();
}


void registerNodeAndComponent(CppBindModule<LuaBinding> &module)
{
    auto component = module.beginClass<Component>("Component");
    REG_METHOD(component, Component, getTypeId);
    REG_METHOD(component, Component, getTags);
    REG_METHOD(component, Component, setTags);
    REG_METHOD(component, Component, getNode);
    component.endClass();

    auto node = module.beginClass<Node>("Node");

    REG_METHOD(node, Node, getId);
    REG_METHOD(node, Node, getScene);
    
    REG_FREE_FUNC_AS_METHOD(node, addScriptComponent);
    REG_FREE_FUNC_AS_METHOD(node, removeScriptComponent);
    REG_FREE_FUNC_AS_METHOD(node, findComponent);
    
    node.addFunction("lala", lala);
    
    REG_FREE_FUNC_AS_METHOD(node, addComponent);
    REG_FREE_FUNC_AS_METHOD(node, removeComponent);
    
    node.endClass();
}
