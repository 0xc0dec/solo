#include <vector>
#include <map>
#include "SoloLuaScriptManager.h"
#include "SoloLuaScriptComponent.h"
#include "SoloEngine.h"
#include "SoloDevice.h"
#include "SoloScene.h"
#include "SoloResourceManager.h"
#include "SoloNode.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include "SoloVector4.h"
#include "SoloQuaternion.h"
#include "SoloMatrix.h"
#include "SoloPlane.h"
#include "SoloFrustum.h"
#include "SoloBoundingBox.h"
#include "SoloBoundingSphere.h"
#include "SoloRay.h"

namespace LuaIntf
{
	LUA_USING_SHARED_PTR_TYPE(std::shared_ptr)
	LUA_USING_LIST_TYPE(std::vector)
	LUA_USING_MAP_TYPE(std::map)
}


using namespace solo;
using namespace LuaIntf;


LuaScriptManager::LuaScriptManager(Engine *engine):
	engine(engine)
{
	lua = LuaState::newState();
	lua.openLibs();
	registerApi();
}


LuaScriptManager::~LuaScriptManager()
{
	lua.close();
}


void registerVector3(CppBindModule& module)
{
	module.beginClass<Vector3>("Vector3")
		.addConstructor(LUA_ARGS(float, float, float))
		.addStaticFunction("zero", &Vector3::zero)
		.addStaticFunction("unit", &Vector3::unit)
		.addStaticFunction("unitX", &Vector3::unitX)
		.addStaticFunction("unitY", &Vector3::unitY)
		.addStaticFunction("unitZ", &Vector3::unitZ)
		.addStaticFunction("angle", &Vector3::angle)
		.addStaticFunction("cross", &Vector3::cross)
		.addStaticFunction("dot", static_cast<float(*)(const Vector3&, const Vector3&)>(&Vector3::dot))
		.addFunction("distance", &Vector3::distance)
		.addFunction("distanceSquared", &Vector3::distanceSquared)
		.addFunction("dot", static_cast<float(Vector3::*)(const Vector3&)const>(&Vector3::dot))
		.addFunction("clamp", &Vector3::clamp)
		.addFunction("length", &Vector3::length)
		.addFunction("lengthSquared", &Vector3::lengthSquared)
		.addFunction("isUnit", &Vector3::isUnit)
		.addFunction("isZero", &Vector3::isZero)
		.addFunction("normalize", &Vector3::normalize)
		.addFunction("normalized", &Vector3::normalized)
		.addVariable("x", &Vector3::x, true)
		.addVariable("y", &Vector3::y, true)
		.addVariable("z", &Vector3::z, true)
		.addFunction("plusScalar", static_cast<Vector3(Vector3::*)(float)const>(&Vector3::operator+))
		.addFunction("plusVector3", static_cast<Vector3(Vector3::*)(const Vector3&)const>(&Vector3::operator+))
		.addFunction("addScalar", static_cast<Vector3&(Vector3::*)(float)>(&Vector3::operator+=))
		.addFunction("addVector3", static_cast<Vector3&(Vector3::*)(const Vector3&)>(&Vector3::operator+=))
	.endClass();
}


void registerVector2(CppBindModule& module)
{
	module.beginClass<Vector2>("Vector2")
		.addConstructor(LUA_ARGS(float, float))
		.addStaticFunction("zero", &Vector2::zero)
		.addStaticFunction("unit", &Vector2::unit)
		.addStaticFunction("unitX", &Vector2::unitX)
		.addStaticFunction("unitY", &Vector2::unitY)
		.addStaticFunction("angle", &Vector2::angle)
		.addStaticFunction("dot", static_cast<float(*)(const Vector2&, const Vector2&)>(&Vector2::dot))
		.addFunction("distance", &Vector2::distance)
		.addFunction("distanceSquared", &Vector2::distanceSquared)
		.addFunction("dot", static_cast<float(Vector2::*)(const Vector2&)const>(&Vector2::dot))
		.addFunction("clamp", &Vector2::clamp)
		.addFunction("length", &Vector2::length)
		.addFunction("lengthSquared", &Vector2::lengthSquared)
		.addFunction("isUnit", &Vector2::isUnit)
		.addFunction("isZero", &Vector2::isZero)
		.addFunction("normalize", &Vector2::normalize)
		.addFunction("normalized", &Vector2::normalized)
		.addVariable("x", &Vector2::x, true)
		.addVariable("y", &Vector2::y, true)
		.addFunction("plusScalar", static_cast<Vector2(Vector2::*)(float)const>(&Vector2::operator+))
		.addFunction("plusVector2", static_cast<Vector2(Vector2::*)(const Vector2&)const>(&Vector2::operator+))
		.addFunction("addScalar", static_cast<Vector2&(Vector2::*)(float)>(&Vector2::operator+=))
		.addFunction("addVector2", static_cast<Vector2&(Vector2::*)(const Vector2&)>(&Vector2::operator+=))
	.endClass();
}

void registerVector4(CppBindModule& module)
{
	module.beginClass<Vector4>("Vector4")
		.addConstructor(LUA_ARGS(float, float, float, float))
		.addStaticFunction("zero", &Vector4::zero)
		.addStaticFunction("unit", &Vector4::unit)
		.addStaticFunction("unitX", &Vector4::unitX)
		.addStaticFunction("unitY", &Vector4::unitY)
		.addStaticFunction("unitZ", &Vector4::unitZ)
		.addStaticFunction("unitW", &Vector4::unitW)
		.addStaticFunction("angle", &Vector4::angle)
		.addStaticFunction("dot", static_cast<float(*)(const Vector4&, const Vector4&)>(&Vector4::dot))
		.addFunction("distance", &Vector4::distance)
		.addFunction("distanceSquared", &Vector4::distanceSquared)
		.addFunction("dot", static_cast<float(Vector4::*)(const Vector4&)const>(&Vector4::dot))
		.addFunction("clamp", &Vector4::clamp)
		.addFunction("length", &Vector4::length)
		.addFunction("lengthSquared", &Vector4::lengthSquared)
		.addFunction("isUnit", &Vector4::isUnit)
		.addFunction("isZero", &Vector4::isZero)
		.addFunction("normalize", &Vector4::normalize)
		.addFunction("normalized", &Vector4::normalized)
		.addVariable("x", &Vector4::x, true)
		.addVariable("y", &Vector4::y, true)
		.addVariable("z", &Vector4::z, true)
		.addVariable("w", &Vector4::w, true)
		.addFunction("plusScalar", static_cast<Vector4(Vector4::*)(float)const>(&Vector4::operator+))
		.addFunction("plusVector4", static_cast<Vector4(Vector4::*)(const Vector4&)const>(&Vector4::operator+))
		.addFunction("addScalar", static_cast<Vector4&(Vector4::*)(float)>(&Vector4::operator+=))
		.addFunction("addVector4", static_cast<Vector4&(Vector4::*)(const Vector4&)>(&Vector4::operator+=))
	.endClass();
}


void registerMatrix(CppBindModule& module)
{
	module.beginClass<Matrix>("Matrix")
		.addConstructor(LUA_ARGS())
		.addStaticFunction("identity", &Matrix::identity)
		.addStaticFunction("zero", &Matrix::zero)
		.addStaticFunction("createLookAt", static_cast<Matrix(*)(const Vector3&, const Vector3&, const Vector3&)>(&Matrix::createLookAt))
		.addStaticFunction("createLookAt2", static_cast<Matrix(*)(float, float, float, float, float, float, float, float, float)>(&Matrix::createLookAt))
		.addStaticFunction("createPerspective", &Matrix::createPerspective)
		.addStaticFunction("createOrthographic", &Matrix::createOrthographic)
		.addStaticFunction("createOrthographicOffCenter", &Matrix::createOrthographicOffCenter)
		.addStaticFunction("createBillboard", static_cast<Matrix(*)(const Vector3&, const Vector3&, const Vector3&)>(&Matrix::createBillboard))
		.addStaticFunction("createBillboard2", static_cast<Matrix(*)(const Vector3&, const Vector3&, const Vector3&, const Vector3&)>(&Matrix::createBillboard))
		.addStaticFunction("createReflection", &Matrix::createReflection)
		.addStaticFunction("createScale", static_cast<Matrix(*)(const Vector3&)>(&Matrix::createScale))
		.addStaticFunction("createScale2", static_cast<Matrix(*)(float, float, float)>(&Matrix::createScale))
		.addStaticFunction("createRotation", static_cast<Matrix(*)(const Quaternion&)>(&Matrix::createRotation))
		.addStaticFunction("createRotationAxisAngle", static_cast<Matrix(*)(const Vector3&, float)>(&Matrix::createRotation))
		.addStaticFunction("createRotationX", &Matrix::createRotationX)
		.addStaticFunction("createRotationY", &Matrix::createRotationY)
		.addStaticFunction("createRotationZ", &Matrix::createRotationZ)
		.addStaticFunction("createTranslation", static_cast<Matrix(*)(const Vector3&)>(&Matrix::createTranslation))
		.addStaticFunction("createTranslation2", static_cast<Matrix(*)(float, float, float)>(&Matrix::createTranslation))
		.addFunction("decompose", &Matrix::decompose)
		.addFunction("getDeterminant", &Matrix::getDeterminant)
		.addFunction("getScale", &Matrix::getScale)
		.addFunction("getRotation", &Matrix::getRotation)
		.addFunction("getTranslation", &Matrix::getTranslation)
		.addFunction("getUpVector", &Matrix::getUpVector)
		.addFunction("getDownVector", &Matrix::getDownVector)
		.addFunction("getLeftVector", &Matrix::getLeftVector)
		.addFunction("getRightVector", &Matrix::getRightVector)
		.addFunction("getForwardVector", &Matrix::getForwardVector)
		.addFunction("getBackVector", &Matrix::getBackVector)
		.addFunction("invert", &Matrix::invert)
		.addFunction("transpose", &Matrix::transpose)
		.addFunction("rotate", static_cast<void(Matrix::*)(const Quaternion&)>(&Matrix::rotate))
		.addFunction("rotateAxisAngle", static_cast<void(Matrix::*)(const Vector3&, float)>(&Matrix::rotate))
		.addFunction("rotateX", &Matrix::rotateX)
		.addFunction("rotateY", &Matrix::rotateY)
		.addFunction("rotateZ", &Matrix::rotateZ)
		.addFunction("scale", static_cast<void(Matrix::*)(float)>(&Matrix::scale))
		.addFunction("scale2", static_cast<void(Matrix::*)(float, float, float)>(&Matrix::scale))
		.addFunction("scale3", static_cast<void(Matrix::*)(const Vector3&)>(&Matrix::scale))
		.addFunction("translate", static_cast<void(Matrix::*)(float, float, float)>(&Matrix::translate))
		.addFunction("translate2", static_cast<void(Matrix::*)(const Vector3&)>(&Matrix::translate))
		.addFunction("setIdentity", &Matrix::setIdentity)
		.addFunction("setZero", &Matrix::setZero)
		.addFunction("transformPoint", &Matrix::transformPoint)
		.addFunction("transformVector3Direction", static_cast<Vector3(Matrix::*)(const Vector3&)const>(&Matrix::transformDirection))
		.addFunction("transformVector4Direction", static_cast<Vector4(Matrix::*)(const Vector4&)const>(&Matrix::transformDirection))
		.addFunction("plusScalar", static_cast<Matrix(Matrix::*)(float)const>(&Matrix::operator+))
		.addFunction("plusMatrix", static_cast<Matrix(Matrix::*)(const Matrix&)const>(&Matrix::operator+))
		.addFunction("addScalar", static_cast<Matrix&(Matrix::*)(float)>(&Matrix::operator+=))
		.addFunction("addMatrix", static_cast<Matrix&(Matrix::*)(const Matrix&)>(&Matrix::operator+=))
		.addFunction("negative", static_cast<Matrix(Matrix::*)()const>(&Matrix::operator-))
		.addFunction("minusScalar", static_cast<Matrix(Matrix::*)(float)const>(&Matrix::operator-))
		.addFunction("minusMatrix", static_cast<Matrix(Matrix::*)(const Matrix&)const>(&Matrix::operator-))
		.addFunction("substractScalar", static_cast<Matrix&(Matrix::*)(float)>(&Matrix::operator-=))
		.addFunction("substractMatrix", static_cast<Matrix&(Matrix::*)(const Matrix&)>(&Matrix::operator-=))
		.addFunction("productScalar", static_cast<Matrix(Matrix::*)(float)const>(&Matrix::operator*))
		.addFunction("productMatrix", static_cast<Matrix(Matrix::*)(const Matrix&)const>(&Matrix::operator*))
		.addFunction("multScalar", static_cast<Matrix&(Matrix::*)(float)>(&Matrix::operator*=))
		.addFunction("multMatrix", static_cast<Matrix&(Matrix::*)(const Matrix&)>(&Matrix::operator*=))
	.endClass();
}


void registerQuaternion(CppBindModule& module)
{
	module.beginClass<Quaternion>("Quaternion")
		.addConstructor(LUA_ARGS())
		.addStaticFunction("zero", &Quaternion::zero)
		.addStaticFunction("identity", &Quaternion::identity)
		.addStaticFunction("createFromAxisAngle", &Quaternion::createFromAxisAngle)
		.addStaticFunction("createFromRotationMatrix", &Quaternion::createFromRotationMatrix)
		.addStaticFunction("lerp", &Quaternion::lerp)
		.addStaticFunction("slerp", &Quaternion::slerp)
		.addStaticFunction("squad", &Quaternion::squad)
		.addFunction("isIdentity", &Quaternion::isIdentity)
		.addFunction("isZero", &Quaternion::isZero)
		.addFunction("conjugate", &Quaternion::conjugate)
		.addFunction("inverse", &Quaternion::inverse)
		.addFunction("normalized", &Quaternion::normalized)
		.addFunction("normalize", &Quaternion::normalize)
		.addFunction("toAxisAngle", &Quaternion::toAxisAngle)
		.addFunction("product", &Quaternion::operator*)
		.addFunction("mult", &Quaternion::operator*=)
		.addVariable("x", &Quaternion::x)
		.addVariable("y", &Quaternion::y)
		.addVariable("z", &Quaternion::z)
		.addVariable("w", &Quaternion::w)
	.endClass();
}


void registerPlane(CppBindModule& module)
{
	module.beginClass<Plane>("Plane")
		.addConstructor(LUA_ARGS(const Vector3&, float))
		.addConstant("Intersection_Intersecting", PlaneIntersection::Intersecting)
		.addConstant("Intersection_Front", PlaneIntersection::Front)
		.addConstant("Intersection_Back", PlaneIntersection::Back)
		.addStaticFunction("intersection", &Plane::intersection)
		.addFunction("getNormal", &Plane::getNormal)
		.addFunction("setNormal", static_cast<void(Plane::*)(const Vector3&)>(&Plane::setNormal))
		.addFunction("getDistance", &Plane::getDistance)
		.addFunction("getDistanceToPoint", &Plane::getDistanceToPoint)
		.addFunction("setDistance", &Plane::setDistance)
		.addFunction("isParallel", &Plane::isParallel)
		.addFunction("transform", &Plane::transform)
	.endClass();
}


void registerFrustum(CppBindModule& module)
{
	module.beginClass<Frustum>("Frustum")
		.addConstructor(LUA_ARGS())
		.addFunction("getNearPlane", &Frustum::getNearPlane)
		.addFunction("getFarPlane", &Frustum::getFarPlane)
		.addFunction("getLeftPlane", &Frustum::getLeftPlane)
		.addFunction("getRightPlane", &Frustum::getRightPlane)
		.addFunction("getTopPlane", &Frustum::getTopPlane)
		.addFunction("getBottomPlane", &Frustum::getBottomPlane)
		.addFunction("getMatrix", &Frustum::getMatrix)
		.addFunction("setMatrix", &Frustum::setMatrix)
		.addFunction("getCorners", &Frustum::getCorners)
		.addFunction("getNearCorners", &Frustum::getNearCorners)
		.addFunction("getFarCorners", &Frustum::getFarCorners)
		.addFunction("intersectsPoint", &Frustum::intersectsPoint)
		.addFunction("intersectsBoundingBox", &Frustum::intersectsBoundingBox)
		.addFunction("intersectsBoundingSphere", &Frustum::intersectsBoundingSphere)
		.addFunction("intersectsRay", &Frustum::intersectsRay)
		.addFunction("getPlaneIntersection", &Frustum::getPlaneIntersection)
	.endClass();
}


void registerBoundingBox(CppBindModule& module)
{
	module.beginClass<BoundingBox>("BoundingBox")
		.addConstructor(LUA_ARGS(const Vector3&, const Vector3&))
		.addStaticFunction("empty", &BoundingBox::empty)
		.addFunction("getCenter", &BoundingBox::getCenter)
		.addFunction("getCorners", &BoundingBox::getCorners)
		.addFunction("intersectsBoundingSphere", &BoundingBox::intersectsBoundingSphere)
		.addFunction("intersectsBoundingBox", &BoundingBox::intersectsBoundingBox)
		.addFunction("intersectsFrustum", &BoundingBox::intersectsFrustum)
		.addFunction("getRayIntersection", &BoundingBox::getRayIntersection)
		.addFunction("getPlaneIntersection", &BoundingBox::getPlaneIntersection)
		.addFunction("isEmpty", &BoundingBox::isEmpty)
		.addFunction("mergeBoundingBox", &BoundingBox::mergeBoundingBox)
		.addFunction("mergeBoundingSphere", &BoundingBox::mergeBoundingSphere)
		.addFunction("transform", &BoundingBox::transform)
		.addVariable("min", &BoundingBox::min)
		.addVariable("max", &BoundingBox::max)
	.endClass();
}


void registerRay(CppBindModule& module)
{
	module.beginClass<Ray>("Ray")
		.addConstructor(LUA_ARGS(const Vector3&, const Vector3&))
	.endClass();
}


void registerBoundingSphere(CppBindModule& module)
{
	module.beginClass<BoundingSphere>("BoundingSphere")
		.addConstructor(LUA_ARGS(const Vector3&, float))
		.addStaticFunction("empty", &BoundingSphere::empty)
		.addFunction("intersectsBoundingSphere", &BoundingSphere::intersectsBoundingSphere)
		.addFunction("intersectsBoundingBox", &BoundingSphere::intersectsBoundingBox)
		.addFunction("intersectsFrustum", &BoundingSphere::intersectsFrustum)
		.addFunction("getRayIntersection", &BoundingSphere::getRayIntersection)
		.addFunction("getPlaneIntersection", &BoundingSphere::getPlaneIntersection)
		.addFunction("isEmpty", &BoundingSphere::isEmpty)
		.addFunction("mergeBoundingBox", &BoundingSphere::mergeBoundingBox)
		.addFunction("mergeBoundingSphere", &BoundingSphere::mergeBoundingSphere)
		.addFunction("transform", &BoundingSphere::transform)
		.addVariable("center", &BoundingSphere::center)
		.addVariable("radius", &BoundingSphere::radius)
	.endClass();
}


void registerDevice(CppBindModule& module)
{
	module.beginClass<Device>("Device")
		.addFunction("getWindowTitle", &Device::getWindowTitle)
		.addFunction("setWindowTitle", &Device::setWindowTitle)
		.addFunction("getLifetime", &Device::getLifetime)
	.endClass();
}


void registerResourceManager(CppBindModule& module)
{
	module.beginClass<ResourceManager>("ResourceManager")
		// TODO
	.endClass();
}


void registerNode(CppBindModule& module)
{
	module.beginClass<Node>("Node")
		.addFunction("getScene", &Node::getScene)
		.addFunction("getId", &Node::getId)
		.addFunction("addComponent", LuaScriptComponent::addComponent)
		.addFunction("removeComponent", &LuaScriptComponent::removeComponent)
		.addFunction("removeAllComponents", &Node::removeAllComponents)
	.endClass();
}


void registerScene(CppBindModule& module)
{
	module.beginClass<Scene>("Scene")
		.addFunction("createNode", &Scene::createNode)
	.endClass();
}


void registerEngine(CppBindModule& module)
{
	module.beginClass<Engine>("Engine")
		.addFunction("getDevice", &Engine::getDevice)
		.addFunction("getScene", &Engine::getScene)
	.endClass();
}


void LuaScriptManager::registerApi()
{
	auto module = LuaBinding(lua).beginModule("solo");
	registerVector2(module);
	registerVector3(module);
	registerVector4(module);
	registerRay(module);
	registerBoundingBox(module);
	registerBoundingSphere(module);
	registerFrustum(module);
	registerQuaternion(module);
	registerPlane(module);
	registerMatrix(module);
	registerDevice(module);
	registerResourceManager(module);
	registerNode(module);
	registerScene(module);
	registerEngine(module);
	module.addVariable("engine", engine, false);
	module.endModule();
}


void LuaScriptManager::execute(const std::string& code)
{
	lua.doString(code.c_str());
}


void LuaScriptManager::executeFile(const std::string& path)
{
	lua.doFile(path.c_str());
}