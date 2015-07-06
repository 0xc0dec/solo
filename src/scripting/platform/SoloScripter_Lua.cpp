//#include "SoloScripter_Lua.h"
//#include "SoloEngine.h"
//#include "SoloDevice.h"
//#include "SoloFileSystem.h"
//#include "SoloScene.h"
//#include "SoloNode.h"
//#include "SoloScriptComponent_Lua.h"
//#include "SoloTransform.h"
//#include "SoloCamera.h"
//
//using namespace solo;
//
//
//Scripter_Lua::Scripter_Lua()
//{
//	engine = NEW<LuaContext>();
//	registerScriptApi();
//}
//
//
//void Scripter_Lua::execString(const std::string& script)
//{
//	engine->executeCode(script);
//}
//
//
//void Scripter_Lua::execFile(const std::string& scriptFilePath)
//{
//	engine->executeCode(Engine::get()->getFileSystem()->readText(scriptFilePath));
//}
//
//
//LuaContext* Scripter_Lua::getEngine() const
//{
//	return engine.get();
//}
//
//
//void Scripter_Lua::registerScriptApi()
//{
//	// Engine
//	engine->writeVariable("engine", Engine::get());
//	engine->registerFunction("getDevice", &Engine::getDevice);
//	engine->registerFunction("getScene", &Engine::getScene);
//
//	// Device
//	engine->registerFunction("getWindowTitle", &Device::getWindowTitle);
//	engine->registerFunction("setWindowTitle", &Device::setWindowTitle);
//
//	// Scene
//	engine->registerFunction("createNode", &Scene::createNode);
//
//	// Transform
//	engine->registerFunction("getParent", &Transform::getParent);
//	engine->registerFunction("setParent", &Transform::setParent);
//	engine->registerFunction("getChild", &Transform::getChild);
//	engine->registerFunction("getChildrenCount", &Transform::getChildrenCount);
//	engine->registerFunction("removeChildrent", &Transform::removeChildren);
//
//	// Camera
//	engine->registerFunction("setClearColor", &Camera::setClearColor);
//	engine->registerFunction("setViewport", &Camera::setViewport);
//	engine->registerFunction("resetViewport", &Camera::resetViewport);
//	engine->registerFunction("getViewport", &Camera::getViewport);
//
//	// Node
//	engine->registerFunction("getId", &Node::getId);
//	engine->registerFunction<Node, void(const std::string&)>("addComponent", &ScriptComponent_Lua::addComponent);
//	engine->registerFunction<Node, void(const std::string&)>("removeComponent", &ScriptComponent_Lua::removeComponent);
//	engine->registerFunction<Node, Transform*(void)>("findTransform", [](Node& node) { return node.findComponent<Transform>(); });
//	engine->registerFunction<Node, Camera*(void)>("findCamera", [](Node& node) { return node.findComponent<Camera>(); });
//	engine->registerFunction<Node, Camera*(void)>("addCamera", [](Node& node) { return node.addComponent<Camera>(); });
//}