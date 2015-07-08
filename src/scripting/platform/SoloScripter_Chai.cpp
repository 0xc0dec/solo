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
#include "SoloVector4.h"
#include "SoloQuaternion.h"
#include "SoloTransform.h"
#include "SoloCamera.h"
#include "SoloModel.h"
#include "SoloMesh.h"
#include "SoloTexture.h"
#include "SoloTexture2D.h"
#include "SoloRenderState.h"
#include "SoloMaterialParameter.h"
#include "SoloRenderContext.h"
#include "SoloMaterial.h"
#include "SoloEffect.h"
#include <chaiscript.hpp>
#include <chaiscript_stdlib.hpp>

using namespace solo;
using namespace chaiscript;


Scripter_Chai::Scripter_Chai()
{
	engine = NEW<ChaiScript>(Std_Lib::library());
	registerScriptApi();
}


void Scripter_Chai::execString(const std::string& script)
{
	try
	{
		engine->eval(script);
	}
	catch (exception::eval_error &e)
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
	catch (exception::eval_error &e)
	{
		THROW_FMT(EngineException, e.pretty_print());
	}
}


shared<ChaiScript> Scripter_Chai::getEngine() const
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
	engine->add_global(var(Engine::get()), "engine");

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
	engine->add(fun(&ResourceManager::findEffect), "findEffect");
	engine->add(fun(&ResourceManager::findMaterial), "findMaterial");
	engine->add(fun(&ResourceManager::findMesh), "findMesh");
	engine->add(fun(&ResourceManager::findModel), "findModel");
	engine->add(fun(&ResourceManager::findRenderTarget), "findRenderTarget");
	engine->add(fun(&ResourceManager::findTexture), "findTexture");
	engine->add(fun(&ResourceManager::getOrCreateRenderTarget), "getOrCreateRenderTarget");
	engine->add(fun(&ResourceManager::getOrLoadTexture), "getOrLoadTexture");
	engine->add(fun(&ResourceManager::getOrLoadMesh), "getOrLoadMesh");
	engine->add(fun(&ResourceManager::getOrCreateEffect), "getOrCreateEffect");
	engine->add(fun(&ResourceManager::getOrCreateMesh), "getOrCreateMesh");
	engine->add(fun(static_cast<shared<Model>(ResourceManager::*)()>(&ResourceManager::getOrCreateModel)), "getOrCreateModel");
	engine->add(fun(static_cast<shared<Model>(ResourceManager::*)(const std::string&)>(&ResourceManager::getOrCreateModel)), "getOrCreateModel");
	engine->add(fun(&ResourceManager::cleanUnusedResources), "cleanUnusedResources");
	engine->add(fun(&ResourceManager::createMaterial), "createMaterial");

	// RenderState
	engine->add(user_type<RenderState>(), "RenderState");
	engine->add(fun(&RenderState::getPolygonFace), "getPolygonFace");
	engine->add(fun(&RenderState::setPolygonFace), "setPolygonFace");

	// Material
	engine->add(user_type<Material>(), "Material");
	engine->add(base_class<RenderState, Material>());
	engine->add(fun(&Material::getEffect), "getEffect");
	engine->add(fun(&Material::getParameter), "getParameter");

	// Model
	engine->add(user_type<Model>(), "Model");
	engine->add(fun(&Model::addMesh), "addMesh");
	engine->add(fun(&Model::getMesh), "getMesh");
	engine->add(fun(&Model::getMeshCount), "getMeshCount");
	engine->add(fun(static_cast<void(Model::*)(Mesh*)>(&Model::removeMesh)), "removeMesh");
	engine->add(fun(static_cast<void(Model::*)(shared<Mesh>)>(&Model::removeMesh)), "removeMesh");

	// Mesh
	engine->add(user_type<Mesh>(), "Mesh");
	engine->add(fun(&Mesh::setVertices), "setVertices");
	engine->add(fun(&Mesh::setNormals), "setNormals");
	engine->add(fun(&Mesh::setUVs), "setUVs");
	engine->add(fun(&Mesh::setIndices), "setIndices");

	// Texture
	engine->add(user_type<Texture>(), "Texture");

	// Texture2D
	engine->add(user_type<Texture2D>(), "Texture2D");
	engine->add(base_class<Texture, Texture2D>());
	engine->add(fun(&Texture2D::getSize), "getSize");
	engine->add(fun(&Texture2D::generateMipmaps), "generateMipmaps");
	engine->add(fun(&Texture2D::getAnisotropyLevel), "getAnisotropyLevel");
	engine->add(fun(&Texture2D::getVerticalWrapMode), "getVerticalWrapMode");
	engine->add(fun(&Texture2D::getHorizontalWrapMode), "getHorizontalWrapMode");
	engine->add(fun(&Texture2D::getMagFilter), "getMagFilter");
	engine->add(fun(&Texture2D::getMinFilter), "getMinFilter");
	engine->add(fun(&Texture2D::setAnisotropyLevel), "setAnisotropyLevel");
	engine->add(fun(&Texture2D::setData), "setData");
	engine->add(fun(&Texture2D::setFilterMode), "setFilterMode");
	engine->add(fun(&Texture2D::setWrapMode), "setWrapMode");
	
	// WrapMode
	engine->add(user_type<WrapMode>(), "WrapMode");
	engine->add_global_const(const_var(WrapMode::Clamp), "WrapMode_Clamp");
	engine->add_global_const(const_var(WrapMode::Repeat), "WrapMode_Repeat");

	// Filter
	engine->add(user_type<Filter>(), "Filter");
	engine->add_global_const(const_var(Filter::Linear), "Filter_Linear");
	engine->add_global_const(const_var(Filter::LinearMipmapLinear), "Filter_LinearMipmapLinear");
	engine->add_global_const(const_var(Filter::LinearMipmapNearest), "Filter_LinearMipmapNearest");
	engine->add_global_const(const_var(Filter::Nearest), "Filter_Nearest");
	engine->add_global_const(const_var(Filter::NearestMipmapLinear), "Filter_NearestMipmapLinear");
	engine->add_global_const(const_var(Filter::NearestMipmapNearest), "Filter_NearestMipmapNearest");

	// ColorFormat
	engine->add(user_type<ColorFormat>(), "ColorFormat");
	engine->add_global_const(const_var(ColorFormat::RGB), "ColorFormat_RGB");
	engine->add_global_const(const_var(ColorFormat::RGBA), "ColorFormat_RGBA");

	// PolygonFace
	engine->add(user_type<PolygonFace>(), "PolygonFace");
	engine->add_global_const(const_var(PolygonFace::CW), "PolygonFace_CW");
	engine->add_global_const(const_var(PolygonFace::All), "PolygonFace_All");
	engine->add_global_const(const_var(PolygonFace::CCW), "PolygonFace_CCW");

	// Scene
	engine->add(user_type<Scene>(), "Scene");
	engine->add(fun(&Scene::createNode), "createNode");

	// Node
	engine->add(user_type<Node>(), "Node");
	engine->add(fun(&Node::getId), "getId");
	engine->add(fun(&ScriptComponent_Chai::addComponent), "addComponent");
	engine->add(fun(&ScriptComponent_Chai::removeComponent), "removeComponent");
	engine->add(fun(&ScriptComponent_Chai::findComponent), "findComponent");

	// MaterialParameter
	engine->add(user_type<MaterialParameter>(), "MaterialParameter");
	engine->add(fun(static_cast<void(MaterialParameter::*)(AutoBinding)>(&MaterialParameter::bindValue)), "bindValue");
	engine->add(fun(static_cast<void(MaterialParameter::*)(std::function<int(const RenderContext& context)>)>(&MaterialParameter::bindValue)), "bindValue");
	engine->add(fun(static_cast<void(MaterialParameter::*)(std::function<float(const RenderContext& context)>)>(&MaterialParameter::bindValue)), "bindValue");
	engine->add(fun(static_cast<void(MaterialParameter::*)(std::function<Vector2(const RenderContext& context)>)>(&MaterialParameter::bindValue)), "bindValue");
	engine->add(fun(static_cast<void(MaterialParameter::*)(std::function<Vector3(const RenderContext& context)>)>(&MaterialParameter::bindValue)), "bindValue");
	engine->add(fun(static_cast<void(MaterialParameter::*)(std::function<Vector4(const RenderContext& context)>)>(&MaterialParameter::bindValue)), "bindValue");
	engine->add(fun(static_cast<void(MaterialParameter::*)(std::function<Matrix(const RenderContext& context)>)>(&MaterialParameter::bindValue)), "bindValue");
	engine->add(fun(static_cast<void(MaterialParameter::*)(std::function<shared<Texture>(const RenderContext& context)>)>(&MaterialParameter::bindValue)), "bindValue");
	engine->add(fun(static_cast<void(MaterialParameter::*)(int)>(&MaterialParameter::setValue)), "setValue");
	engine->add(fun(static_cast<void(MaterialParameter::*)(float)>(&MaterialParameter::setValue)), "setValue");
	engine->add(fun(static_cast<void(MaterialParameter::*)(const Vector2&)>(&MaterialParameter::setValue)), "setValue");
	engine->add(fun(static_cast<void(MaterialParameter::*)(const Vector3&)>(&MaterialParameter::setValue)), "setValue");
	engine->add(fun(static_cast<void(MaterialParameter::*)(const Vector4&)>(&MaterialParameter::setValue)), "setValue");
	engine->add(fun(static_cast<void(MaterialParameter::*)(const Matrix&)>(&MaterialParameter::setValue)), "setValue");
	engine->add(fun(static_cast<void(MaterialParameter::*)(shared<Texture>)>(&MaterialParameter::setValue)), "setValue");
	// TODO other

	// AutoBinding
	engine->add(user_type<AutoBinding>(), "AutoBinding");
	engine->add_global_const(const_var(AutoBinding::None), "AutoBinding_None");
	engine->add_global_const(const_var(AutoBinding::CameraWorldPosition), "AutoBinding_CameraWorldPosition");
	engine->add_global_const(const_var(AutoBinding::InverseTransposedWorldMatrix), "AutoBinding_InverseTransposedWorldMatrix");
	engine->add_global_const(const_var(AutoBinding::InverseTransposedWorldViewMatrix), "AutoBinding_InverseTransposedWorldViewMatrix");
	engine->add_global_const(const_var(AutoBinding::ProjectionMatrix), "AutoBinding_ProjectionMatrix");
	engine->add_global_const(const_var(AutoBinding::ViewMatrix), "AutoBinding_ViewMatrix");
	engine->add_global_const(const_var(AutoBinding::ViewProjectionMatrix), "AutoBinding_ViewProjectionMatrix");
	engine->add_global_const(const_var(AutoBinding::WorldMatrix), "AutoBinding_WorldMatrix");
	engine->add_global_const(const_var(AutoBinding::WorldViewProjection), "AutoBinding_WorldViewProjection");
	engine->add_global_const(const_var(AutoBinding::WorldViewProjectionMatrix), "AutoBinding_WorldViewProjectionMatrix");

	// RenderContext
	engine->add(user_type<RenderContext>(), "RenderContext");

	// Vector2
	engine->add(user_type<Vector2>(), "Vector2");
	engine->add(constructor<Vector2()>(), "Vector2");
	engine->add(constructor<Vector2(float, float)>(), "Vector2");
	engine->add(fun(&Vector2::x), "x");
	engine->add(fun(&Vector2::y), "y");
	engine->add(fun(&Vector2::unitX), "unitVector2X");
	engine->add(fun(&Vector2::unitY), "unitVector2Y");
	engine->add(fun(&Vector2::zero), "zeroVector2");
	engine->add(fun(&Vector2::isZero), "isZero");
	engine->add(fun(static_cast<void(Vector2::*)()>(&Vector2::normalize)), "normalize");
	engine->add(fun(static_cast<void(Vector2::*)(Vector2*)const>(&Vector2::normalize)), "normalize");
	engine->add(fun(&Vector2::normalized), "normalized");
	engine->add(fun(&Vector2::operator*=), "*=");
	engine->add(fun(&Vector2::operator!=), "!=");
	engine->add(fun(&Vector2::operator*), "*");
	engine->add(fun(&Vector2::operator+), "+");
	engine->add(fun(&Vector2::operator+=), "+=");
	engine->add(fun(static_cast<Vector2(Vector2::*)()const>(&Vector2::operator-)), "-=");
	engine->add(fun(static_cast<Vector2(Vector2::*)(const Vector2&)const>(&Vector2::operator-)), "-=");
	engine->add(fun(&Vector2::operator/), "/");
	engine->add(fun(&Vector2::operator<), "<");
	engine->add(fun(&Vector2::operator==), "==");

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
	engine->add(fun(&Vector3::isZero), "isZero");
	engine->add(fun(&Vector3::scale), "scale");
	engine->add(fun(&Vector3::smooth), "smooth");
	engine->add(fun(&Vector3::subtract), "subtract");
	engine->add(fun(&Vector3::operator*=), "*=");
	engine->add(fun(&Vector3::operator!=), "!=");
	engine->add(fun(&Vector3::operator*), "*");
	engine->add(fun(&Vector3::operator+), "+");
	engine->add(fun(&Vector3::operator+=), "+=");
	engine->add(fun(static_cast<Vector3(Vector3::*)()const>(&Vector3::operator-)), "-=");
	engine->add(fun(static_cast<Vector3(Vector3::*)(const Vector3&)const>(&Vector3::operator-)), "-=");
	engine->add(fun(&Vector3::operator/), "/");
	engine->add(fun(&Vector3::operator<), "<");
	engine->add(fun(&Vector3::operator==), "==");

	// Vector4
	engine->add(user_type<Vector4>(), "Vector4");
	engine->add(constructor<Vector4()>(), "Vector4");
	engine->add(constructor<Vector4(float, float, float, float)>(), "Vector4");
	engine->add(fun(&Vector4::x), "x");
	engine->add(fun(&Vector4::y), "y");
	engine->add(fun(&Vector4::z), "z");
	engine->add(fun(&Vector4::unitX), "unitVector4X");
	engine->add(fun(&Vector4::unitY), "unitVector4Y");
	engine->add(fun(&Vector4::unitZ), "unitVector4Z");
	engine->add(fun(&Vector4::zero), "zeroVector4");
	engine->add(fun(static_cast<void(Vector4::*)()>(&Vector4::normalize)), "normalize");
	engine->add(fun(static_cast<void(Vector4::*)(Vector4*)const>(&Vector4::normalize)), "normalize");
	engine->add(fun(&Vector4::normalized), "normalized");
	engine->add(fun(&Vector4::add), "add");
	engine->add(fun(&Vector4::angle), "angle");
	engine->add(fun(&Vector4::clamp), "clamp");
	engine->add(fun(&Vector4::distance), "distance");
	engine->add(fun(&Vector4::distanceSquared), "distanceSquared");
	engine->add(fun(&Vector4::dot), "dot");
	engine->add(fun(&Vector4::isOne), "isOne");
	engine->add(fun(&Vector4::isZero), "isZero");
	engine->add(fun(&Vector4::scale), "scale");
	engine->add(fun(&Vector4::subtract), "subtract");
	engine->add(fun(&Vector4::operator*=), "*=");
	engine->add(fun(&Vector4::operator!=), "!=");
	engine->add(fun(&Vector4::operator*), "*");
	engine->add(fun(&Vector4::operator+), "+");
	engine->add(fun(&Vector4::operator+=), "+=");
	engine->add(fun(static_cast<Vector4(Vector4::*)()const>(&Vector4::operator-)), "-=");
	engine->add(fun(static_cast<Vector4(Vector4::*)(const Vector4&)const>(&Vector4::operator-)), "-=");
	engine->add(fun(&Vector4::operator/), "/");
	engine->add(fun(&Vector4::operator<), "<");
	engine->add(fun(&Vector4::operator==), "==");

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
	engine->add(fun(&Quaternion::operator*), "*");
	engine->add(fun(&Quaternion::operator*=), "*=");

	// Transform
	engine->add(user_type<Transform>(), "Transform");
	engine->add(fun(&Transform::getParent), "getParent");
	engine->add(fun(&Transform::setParent), "setParent");
	engine->add(fun(&Transform::getLocalPosition), "getLocalPosition");
	engine->add(fun(&Transform::getLocalRotation), "getLocalRotation");
	engine->add(fun(&Transform::getLocalScale), "getLocalScale");
	engine->add(fun(&Transform::getWorldPosition), "getWorldPosition");
	engine->add(fun(&Transform::getWorldRotation), "getWorldRotation");
	engine->add(fun(&Transform::getWorldScale), "getWorldScale");
	engine->add(fun(static_cast<void(Transform::*)(float, float, float)>(&Transform::setLocalPosition)), "setLocalPosition");
	engine->add(fun(static_cast<void(Transform::*)(const Vector3&)>(&Transform::setLocalPosition)), "setLocalPosition");
	engine->add(fun(static_cast<void(Transform::*)(const Quaternion&)>(&Transform::setLocalRotation)), "setLocalRotation");
	engine->add(fun(static_cast<void(Transform::*)(const Vector3&, float)>(&Transform::setLocalRotation)), "setLocalRotation");
	engine->add(fun(static_cast<void(Transform::*)(float)>(&Transform::setLocalScale)), "setLocalScale");
	engine->add(fun(static_cast<void(Transform::*)(const Vector3&)>(&Transform::setLocalScale)), "setLocalScale");
	engine->add(fun(static_cast<void(Transform::*)(float, float, float)>(&Transform::setLocalScale)), "setLocalScale");
	engine->add(fun(&Transform::getChild), "getChild");
	engine->add(fun(&Transform::getChildrenCount), "getChildrenCount");
	engine->add(fun(&Transform::getInverseTransposedWorldMatrix), "getInverseTransposedWorldMatrix");
	engine->add(fun(&Transform::getInverseTransposedWorldViewMatrix), "getInverseTransposedWorldViewMatrix");
	engine->add(fun(&Transform::getLocalForward), "getLocalForward");
	engine->add(fun(&Transform::getLocalBack), "getLocalBack");
	engine->add(fun(&Transform::getLocalLeft), "getLocalLeft");
	engine->add(fun(&Transform::getLocalRight), "getLocalRight");
	engine->add(fun(&Transform::getLocalUp), "getLocalUp");
	engine->add(fun(&Transform::getLocalDown), "getLocalDown");
	engine->add(fun(&Transform::getMatrix), "getMatrix");
	engine->add(fun(&Transform::getWorldMatrix), "getWorldMatrix");
	engine->add(fun(&Transform::getWorldViewMatrix), "getWorldViewMatrix");
	engine->add(fun(&Transform::getWorldViewProjectionMatrix), "getWorldViewProjectionMatrix");
	engine->add(fun(static_cast<void(Transform::*)(const Quaternion&, TransformSpace)>(&Transform::rotate)), "rotate");
	engine->add(fun(static_cast<void(Transform::*)(const Vector3&, float, TransformSpace)>(&Transform::rotate)), "rotate");
	engine->add(fun(static_cast<void(Transform::*)(float)>(&Transform::scaleLocal)), "scaleLocal");
	engine->add(fun(static_cast<void(Transform::*)(const Vector3&)>(&Transform::scaleLocal)), "scaleLocal");
	engine->add(fun(static_cast<void(Transform::*)(float, float, float)>(&Transform::scaleLocal)), "scaleLocal");
	engine->add(fun(&Transform::removeChildren), "removeChildren");
	engine->add(fun(static_cast<void(Transform::*)(const Vector3&)>(&Transform::translateLocal)), "translateLocal");
	engine->add(fun(static_cast<void(Transform::*)(float, float, float)>(&Transform::translateLocal)), "translateLocal");
	engine->add(fun(&Transform::transformPoint), "transformPoint");
	engine->add(fun(&Transform::transformDirection), "transformDirection");

	// KeyCode
	engine->add(user_type<KeyCode>(), "KeyCode");
	engine->add_global_const(const_var(KeyCode::A), "KeyCode_A");
	engine->add_global_const(const_var(KeyCode::B), "KeyCode_B");
	engine->add_global_const(const_var(KeyCode::C), "KeyCode_C");
	engine->add_global_const(const_var(KeyCode::D), "KeyCode_D");
	engine->add_global_const(const_var(KeyCode::E), "KeyCode_E");
	engine->add_global_const(const_var(KeyCode::F), "KeyCode_F");
	engine->add_global_const(const_var(KeyCode::G), "KeyCode_G");
	engine->add_global_const(const_var(KeyCode::H), "KeyCode_H");
	engine->add_global_const(const_var(KeyCode::I), "KeyCode_I");
	engine->add_global_const(const_var(KeyCode::J), "KeyCode_J");
	engine->add_global_const(const_var(KeyCode::K), "KeyCode_K");
	engine->add_global_const(const_var(KeyCode::L), "KeyCode_L");
	engine->add_global_const(const_var(KeyCode::M), "KeyCode_M");
	engine->add_global_const(const_var(KeyCode::N), "KeyCode_N");
	engine->add_global_const(const_var(KeyCode::O), "KeyCode_O");
	engine->add_global_const(const_var(KeyCode::P), "KeyCode_P");
	engine->add_global_const(const_var(KeyCode::Q), "KeyCode_Q");
	engine->add_global_const(const_var(KeyCode::R), "KeyCode_R");
	engine->add_global_const(const_var(KeyCode::S), "KeyCode_S");
	engine->add_global_const(const_var(KeyCode::T), "KeyCode_T");
	engine->add_global_const(const_var(KeyCode::U), "KeyCode_U");
	engine->add_global_const(const_var(KeyCode::V), "KeyCode_V");
	engine->add_global_const(const_var(KeyCode::W), "KeyCode_W");
	engine->add_global_const(const_var(KeyCode::X), "KeyCode_X");
	engine->add_global_const(const_var(KeyCode::Y), "KeyCode_Y");
	engine->add_global_const(const_var(KeyCode::Z), "KeyCode_Z");
	engine->add_global_const(const_var(KeyCode::UpArrow), "KeyCode_UpArrow");
	engine->add_global_const(const_var(KeyCode::DownArrow), "KeyCode_DownArrow");
	engine->add_global_const(const_var(KeyCode::LeftArrow), "KeyCode_LeftArrow");
	engine->add_global_const(const_var(KeyCode::RightArrow), "KeyCode_RightArrow");
	engine->add_global_const(const_var(KeyCode::Escape), "KeyCode_Escape");

	// MouseButton
	engine->add(user_type<MouseButton>(), "MouseButton");
	engine->add_global_const(const_var(MouseButton::Left), "MouseButton_Left");
	engine->add_global_const(const_var(MouseButton::Right), "MouseButton_Right");
	engine->add_global_const(const_var(MouseButton::Middle), "MouseButton_Middle");

	// Transform space
	engine->add(user_type<TransformSpace>(), "TransformSpace");
	engine->add_global_const(const_var(TransformSpace::Parent), "TransformSpace_Parent");
	engine->add_global_const(const_var(TransformSpace::Self), "TransformSpace_Self");
	engine->add_global_const(const_var(TransformSpace::World), "TransformSpace_World");
}
