#include "SoloScripter_Chai.h"
#include "SoloScriptComponent_Chai.h"
#include "SoloEngine.h"
#include "SoloDevice.h"
#include "SoloScene.h"
#include "SoloNode.h"
#include "SoloResourceManager.h"
#include "SoloTexture.h"
#include "SoloFileSystem.h"
#include "SoloNode.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include "SoloQuaternion.h"
#include "SoloTransform.h"
#include <chaiscript.hpp>
#include <chaiscript_stdlib.hpp>

using namespace solo;


Scripter_Chai::Scripter_Chai()
{
	engine = NEW<chaiscript::ChaiScript>(chaiscript::Std_Lib::library());
	registerScriptApi();
}


void Scripter_Chai::execString(const std::string& script)
{
	try
	{
		engine->eval(script);
	}
	catch (chaiscript::exception::eval_error &e)
	{
		THROW_FMT(EngineException, e.pretty_print());
	}
}


void Scripter_Chai::execFile(const std::string& scriptFileName)
{
	try
	{
		engine->eval_file(scriptFileName);
	}
	catch (chaiscript::exception::eval_error &e)
	{
		THROW_FMT(EngineException, e.pretty_print());
	}
}


shared<chaiscript::ChaiScript> Scripter_Chai::getEngine() const
{
	return engine;
}


void Scripter_Chai::registerScriptApi()
{
	using namespace chaiscript;

	// Engine
	engine->add(user_type<Engine>(), "Engine");
	engine->add(fun(&Engine::getDevice), "getDevice");
	engine->add(fun(&Engine::getFileSystem), "getFileSystem");
	engine->add(fun(&Engine::getResourceManager), "getResourceManager");
	engine->add(fun(&Engine::getScene), "getScene");
	engine->add(var(Engine::get()), "engine");

	// Device
	engine->add(user_type<Device>(), "Device");
	engine->add(fun(&Device::getWindowTitle), "getWindowTitle");
	engine->add(fun(&Device::setWindowTitle), "setWindowTitle");
	engine->add(fun(&Device::requestShutdown), "requestShutdown");
	engine->add(fun(&Device::shutdownRequested), "shutdownRequested");
	engine->add(fun(&Device::getMouseMotion), "getMouseMotion");
	engine->add(fun(&Device::setCursorCaptured), "setCursorCaptured");
	engine->add(fun(&Device::getCanvasSize), "getCanvasSize");
	engine->add(fun(&Device::getLifetime), "getLifetime");
	engine->add(fun(&Device::getTimeDelta), "getTimeDelta");
	engine->add(fun(&Device::isKeyPressed), "isKeyPressed");
	engine->add(fun(&Device::isKeyReleased), "isKeyReleased");
	engine->add(fun(&Device::isMouseButtonDown), "isMouseButtonDown");
	engine->add(fun(&Device::isMouseButtonReleased), "isMouseButtonReleased");

	// File system
	engine->add(user_type<FileSystem>(), "FileSystem");

	// Resource manager
	engine->add(user_type<ResourceManager>(), "ResourceManager");

	// Scene
	engine->add(user_type<Scene>(), "Scene");
	engine->add(fun(&Scene::createNode), "createNode");

	// Node
	engine->add(user_type<Node>(), "Node");
	engine->add(fun(&Node::getId), "getId");
	engine->add(fun(&ScriptComponent_Chai::addComponent), "addComponent");
	engine->add(fun(&ScriptComponent_Chai::removeComponent), "removeComponent");
	engine->add(fun(&ScriptComponent_Chai::findComponent), "findComponent");

	// Vector2
	engine->add(user_type<Vector2>(), "Vector2");
	engine->add(constructor<Vector2()>(), "Vector2");
	engine->add(constructor<Vector2(float, float)>(), "Vector2");
	engine->add(fun(&Vector2::x), "x");
	engine->add(fun(&Vector2::y), "y");
	engine->add(fun(&Vector3::unitX), "unitVector2X");
	engine->add(fun(&Vector3::unitY), "unitVector2Y");
	engine->add(fun(&Vector3::zero), "zeroVector2");
	engine->add(fun(&Vector3::isZero), "isZero");
	engine->add(fun(static_cast<void(Vector2::*)()>(&Vector2::normalize)), "normalize");
	engine->add(fun(static_cast<void(Vector2::*)(Vector2*)const>(&Vector2::normalize)), "normalize");
	engine->add(fun(&Vector2::normalized), "normalized");

	// Vector3
	engine->add(user_type<Vector3>(), "Vector3");
	engine->add(constructor<Vector3()>(), "Vector3");
	engine->add(constructor<Vector3(float, float, float)>(), "Vector3");
	engine->add(fun(&Vector3::x), "x");
	engine->add(fun(&Vector3::y), "y");
	engine->add(fun(&Vector3::z), "z");
	engine->add(fun(&Vector3::unitX), "unitVector3X");
	engine->add(fun(&Vector3::unitY), "unitVector3Y");
	engine->add(fun(&Vector3::unitZ), "unitVector3Z");
	engine->add(fun(&Vector3::zero), "zeroVector3");
	engine->add(fun(&Vector3::isZero), "isZero");
	engine->add(fun(static_cast<void(Vector3::*)()>(&Vector3::normalize)), "normalize");
	engine->add(fun(static_cast<void(Vector3::*)(Vector3*)const>(&Vector3::normalize)), "normalize");
	engine->add(fun(&Vector3::normalized), "normalized");
	engine->add(fun(&Vector3::add), "add");
	engine->add(fun(&Vector3::angle), "angle");
	engine->add(fun(&Vector3::clamp), "clamp");
	engine->add(fun(&Vector3::cross), "cross");
	engine->add(fun(&Vector3::distance), "distance");
	engine->add(fun(&Vector3::distanceSquared), "distanceSquared");
	engine->add(fun(&Vector3::dot), "dot");
	engine->add(fun(&Vector3::isOne), "isOne");
	engine->add(fun(&Vector3::scale), "scale");
	engine->add(fun(&Vector3::smooth), "smooth");
	engine->add(fun(&Vector3::subtract), "subtract");

	// Quaternion
	engine->add(user_type<Quaternion>(), "Quaternion");
	engine->add(constructor<Quaternion()>(), "Quaternion");
	engine->add(constructor<Quaternion(float, float, float, float)>(), "Quaternion");
	engine->add(fun(&Quaternion::x), "x");
	engine->add(fun(&Quaternion::y), "y");
	engine->add(fun(&Quaternion::z), "z");
	engine->add(fun(&Quaternion::w), "w");
	engine->add(fun(&Quaternion::identity), "identityQuaternion");
	engine->add(fun(&Quaternion::zero), "zeroQuaternion");
	engine->add(fun(static_cast<void(*)(const Vector3&, float, Quaternion*)>(&Quaternion::createFromAxisAngle)), "createQuaternionFromAxisAngle");
	engine->add(fun(static_cast<Quaternion(*)(const Vector3&, float)>(&Quaternion::createFromAxisAngle)), "createQuaternionFromAxisAngle");
	engine->add(fun(static_cast<void(*)(const Matrix&, Quaternion*)>(&Quaternion::createFromRotationMatrix)), "createQuaternionFromRotationMatrix");
	engine->add(fun(static_cast<Quaternion(*)(const Matrix&)>(&Quaternion::createFromRotationMatrix)), "createQuaternionFromRotationMatrix");
	engine->add(fun(static_cast<void(*)(const Quaternion&, const Quaternion&, float, Quaternion*)>(&Quaternion::lerp)), "lertQuaternion");
	engine->add(fun(static_cast<void(*)(const Quaternion&, const Quaternion&, float, Quaternion*)>(&Quaternion::slerp)), "slertQuaternion");
	engine->add(fun(static_cast<void(*)(const Quaternion&, const Quaternion&, const Quaternion&, const Quaternion&, float, Quaternion*)>(&Quaternion::squad)), "squadQuaternion");
	engine->add(fun(static_cast<bool(Quaternion::*)()>(&Quaternion::inverse)), "inverse");
	engine->add(fun(static_cast<void(*)(const Quaternion&, const Quaternion&, Quaternion*)>(&Quaternion::multiply)), "multiplyQuaternions");
	engine->add(fun(&Quaternion::multiply), "multiply");
	engine->add(fun(static_cast<void(Quaternion::*)(Quaternion*)const>(&Quaternion::normalize)), "normalize");
	engine->add(fun(static_cast<void(Quaternion::*)()>(&Quaternion::normalize)), "normalize");
	engine->add(fun(&Quaternion::setIdentity), "setIdentity");
	engine->add(fun(static_cast<bool(Quaternion::*)(Quaternion*)const>(&Quaternion::inverse)), "inverse");
	engine->add(fun(static_cast<void(Quaternion::*)()>(&Quaternion::conjugate)), "conjugate");
	engine->add(fun(static_cast<void(Quaternion::*)(Quaternion*)const>(&Quaternion::conjugate)), "conjugate");
	engine->add(fun(&Quaternion::isIdentity), "isIdentity");
	engine->add(fun(&Quaternion::isZero), "isZero");
	engine->add(fun(&Quaternion::toAxisAngle), "toAxisAngle");

	// Transform
	engine->add(user_type<Transform>(), "Transform");
	engine->add(fun(&Transform::getLocalPosition), "getLocalPosition");
	engine->add(fun(static_cast<void(Transform::*)(float, float, float)>(&Transform::setLocalPosition)), "setLocalPosition");
	engine->add(fun(static_cast<void(Transform::*)(const Vector3&)>(&Transform::setLocalPosition)), "setLocalPosition");

	// KeyCode
	engine->add(user_type<KeyCode>(), "KeyCode");
	engine->add(const_var(KeyCode::A), "KeyCode_A");
	engine->add(const_var(KeyCode::B), "KeyCode_B");
	engine->add(const_var(KeyCode::C), "KeyCode_C");
	engine->add(const_var(KeyCode::D), "KeyCode_D");
	engine->add(const_var(KeyCode::E), "KeyCode_E");
	engine->add(const_var(KeyCode::F), "KeyCode_F");
	engine->add(const_var(KeyCode::G), "KeyCode_G");
	engine->add(const_var(KeyCode::H), "KeyCode_H");
	engine->add(const_var(KeyCode::I), "KeyCode_I");
	engine->add(const_var(KeyCode::J), "KeyCode_J");
	engine->add(const_var(KeyCode::K), "KeyCode_K");
	engine->add(const_var(KeyCode::L), "KeyCode_L");
	engine->add(const_var(KeyCode::M), "KeyCode_M");
	engine->add(const_var(KeyCode::N), "KeyCode_N");
	engine->add(const_var(KeyCode::O), "KeyCode_O");
	engine->add(const_var(KeyCode::P), "KeyCode_P");
	engine->add(const_var(KeyCode::Q), "KeyCode_Q");
	engine->add(const_var(KeyCode::R), "KeyCode_R");
	engine->add(const_var(KeyCode::S), "KeyCode_S");
	engine->add(const_var(KeyCode::T), "KeyCode_T");
	engine->add(const_var(KeyCode::U), "KeyCode_U");
	engine->add(const_var(KeyCode::V), "KeyCode_V");
	engine->add(const_var(KeyCode::W), "KeyCode_W");
	engine->add(const_var(KeyCode::X), "KeyCode_X");
	engine->add(const_var(KeyCode::Y), "KeyCode_Y");
	engine->add(const_var(KeyCode::Z), "KeyCode_Z");
	engine->add(const_var(KeyCode::UpArrow), "KeyCode_UpArrow");
	engine->add(const_var(KeyCode::DownArrow), "KeyCode_DownArrow");
	engine->add(const_var(KeyCode::LeftArrow), "KeyCode_LeftArrow");
	engine->add(const_var(KeyCode::RightArrow), "KeyCode_RightArrow");
	engine->add(const_var(KeyCode::Escape), "KeyCode_Escape");

	// MouseButton
	engine->add(user_type<MouseButton>(), "MouseButton");
	engine->add(const_var(MouseButton::Left), "MouseButton_Left");
	engine->add(const_var(MouseButton::Right), "MouseButton_Right");
	engine->add(const_var(MouseButton::Middle), "MouseButton_Middle");
}