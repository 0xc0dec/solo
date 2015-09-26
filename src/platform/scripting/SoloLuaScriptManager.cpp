#include <vector>
#include <map>
#include "SoloLuaScriptManager.h"
#include "SoloLuaScriptComponent.h"
#include "SoloLuaEngineCallback.h"
#include "SoloEngine.h"
#include "SoloDevice.h"
#include "SoloScene.h"
#include "SoloResourceManager.h"
#include "SoloFileSystem.h"
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
#include "SoloTransform.h"
#include "SoloCamera.h"
#include "SoloRenderTarget.h"
#include "SoloEffect.h"
#include "SoloTexture.h"
#include "SoloTexture2D.h"
#include "SoloMaterial.h"
#include "SoloMaterialParameter.h"
#include "SoloMesh.h"
#include "SoloModel.h"


#define REGISTER_VARIABLE(binding, klass, name) binding.addVariable(#name, &klass::name, true)
#define REGISTER_METHOD(binding, klass, name) binding.addFunction(#name, &klass::name)
#define REGISTER_METHOD2(binding, klass, name, argsSpec) binding.addFunction(#name, &klass::name, argsSpec)
#define REGISTER_METHOD_RENAMED(binding, klass, name, nameStr) binding.addFunction(nameStr, &klass::name)
#define REGISTER_OVERLOADED_METHOD(binding, klass, name, nameStr, resultType, modifier, ...) \
	binding.addFunction(nameStr, static_cast<resultType(klass::*)(__VA_ARGS__)modifier>(&klass::name))
#define REGISTER_STATIC_METHOD(binding, klass, name) binding.addStaticFunction(#name, &klass::name)
#define REGISTER_STATIC_OVERLOADED_METHOD(binding, klass, name, nameStr, resultType, modifier, ...) \
	binding.addStaticFunction(nameStr, static_cast<resultType(*)(__VA_ARGS__)modifier>(&klass::name))


namespace LuaIntf
{
	LUA_USING_SHARED_PTR_TYPE(std::shared_ptr)
	LUA_USING_LIST_TYPE(std::vector)
	LUA_USING_MAP_TYPE(std::map)
}


using namespace solo;
using namespace LuaIntf;


LuaScriptManager::LuaScriptManager()
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
	auto vector3 = module.beginClass<Vector3>("Vector3");
	vector3.addConstructor(LUA_ARGS(float, float, float));
	REGISTER_STATIC_OVERLOADED_METHOD(vector3, Vector3, dot, "dot", float, , const Vector3&, const Vector3&);
	REGISTER_VARIABLE(vector3, Vector3, x);
	REGISTER_VARIABLE(vector3, Vector3, y);
	REGISTER_VARIABLE(vector3, Vector3, z);
	REGISTER_STATIC_METHOD(vector3, Vector3, zero);
	REGISTER_STATIC_METHOD(vector3, Vector3, unit);
	REGISTER_STATIC_METHOD(vector3, Vector3, unitX);
	REGISTER_STATIC_METHOD(vector3, Vector3, unitY);
	REGISTER_STATIC_METHOD(vector3, Vector3, unitZ);
	REGISTER_STATIC_METHOD(vector3, Vector3, angle);
	REGISTER_STATIC_METHOD(vector3, Vector3, cross);
	REGISTER_OVERLOADED_METHOD(vector3, Vector3, dot, "dot", float, const, const Vector3&);
	REGISTER_METHOD(vector3, Vector3, distance);
	REGISTER_METHOD(vector3, Vector3, distanceSquared);
	REGISTER_METHOD(vector3, Vector3, clamp);
	REGISTER_METHOD(vector3, Vector3, length);
	REGISTER_METHOD(vector3, Vector3, lengthSquared);
	REGISTER_METHOD(vector3, Vector3, isUnit);
	REGISTER_METHOD(vector3, Vector3, isZero);
	REGISTER_METHOD(vector3, Vector3, normalize);
	REGISTER_METHOD(vector3, Vector3, normalized);
	REGISTER_METHOD(vector3, Vector3, normalized);
	REGISTER_OVERLOADED_METHOD(vector3, Vector3, operator+, "plusScalar", Vector3, const, float);
	REGISTER_OVERLOADED_METHOD(vector3, Vector3, operator+, "plusVector3", Vector3, const, const Vector3&);
	REGISTER_OVERLOADED_METHOD(vector3, Vector3, operator+=, "addScalar", Vector3&,, float);
	REGISTER_OVERLOADED_METHOD(vector3, Vector3, operator+=, "addVector3", Vector3&,, const Vector3&);
	vector3.endClass();
}


void registerVector2(CppBindModule& module)
{
	auto vector2 = module.beginClass<Vector2>("Vector2");
	vector2.addConstructor(LUA_ARGS(float, float));
	REGISTER_STATIC_OVERLOADED_METHOD(vector2, Vector2, dot, "dot", float, , const Vector2&, const Vector2&);
	REGISTER_STATIC_METHOD(vector2, Vector2, zero);
	REGISTER_STATIC_METHOD(vector2, Vector2, unit);
	REGISTER_STATIC_METHOD(vector2, Vector2, unitX);
	REGISTER_STATIC_METHOD(vector2, Vector2, unitY);
	REGISTER_STATIC_METHOD(vector2, Vector2, angle);
	REGISTER_VARIABLE(vector2, Vector2, x);
	REGISTER_VARIABLE(vector2, Vector2, y);
	REGISTER_METHOD(vector2, Vector2, distance);
	REGISTER_METHOD(vector2, Vector2, distanceSquared);
	REGISTER_OVERLOADED_METHOD(vector2, Vector2, dot, "dot", float, const, const Vector2&);
	REGISTER_METHOD(vector2, Vector2, clamp);
	REGISTER_METHOD(vector2, Vector2, length);
	REGISTER_METHOD(vector2, Vector2, lengthSquared);
	REGISTER_METHOD(vector2, Vector2, isUnit);
	REGISTER_METHOD(vector2, Vector2, isZero);
	REGISTER_METHOD(vector2, Vector2, normalize);
	REGISTER_METHOD(vector2, Vector2, normalized);
	REGISTER_OVERLOADED_METHOD(vector2, Vector2, operator+, "plusScalar", Vector2, const, float);
	REGISTER_OVERLOADED_METHOD(vector2, Vector2, operator+, "plusVector2", Vector2, const, const Vector2&);
	REGISTER_OVERLOADED_METHOD(vector2, Vector2, operator+=, "addScalar", Vector2&,, float);
	REGISTER_OVERLOADED_METHOD(vector2, Vector2, operator+=, "addVector2", Vector2&,, const Vector2&);
	vector2.endClass();
}

void registerVector4(CppBindModule& module)
{
	auto vector4 = module.beginClass<Vector4>("Vector4");
	vector4.addConstructor(LUA_ARGS(float, float, float, float));
	REGISTER_VARIABLE(vector4, Vector4, x);
	REGISTER_VARIABLE(vector4, Vector4, y);
	REGISTER_VARIABLE(vector4, Vector4, z);
	REGISTER_VARIABLE(vector4, Vector4, w);
	REGISTER_STATIC_METHOD(vector4, Vector4, zero);
	REGISTER_STATIC_METHOD(vector4, Vector4, unit);
	REGISTER_STATIC_METHOD(vector4, Vector4, unitX);
	REGISTER_STATIC_METHOD(vector4, Vector4, unitY);
	REGISTER_STATIC_METHOD(vector4, Vector4, unitZ);
	REGISTER_STATIC_METHOD(vector4, Vector4, unitW);
	REGISTER_STATIC_METHOD(vector4, Vector4, angle);
	REGISTER_STATIC_OVERLOADED_METHOD(vector4, Vector4, dot, "dot", float, , const Vector4&, const Vector4&);
	REGISTER_METHOD(vector4, Vector4, distance);
	REGISTER_METHOD(vector4, Vector4, distanceSquared);
	REGISTER_OVERLOADED_METHOD(vector4, Vector4, dot, "dot", float, const, const Vector4&);
	REGISTER_METHOD(vector4, Vector4, clamp);
	REGISTER_METHOD(vector4, Vector4, length);
	REGISTER_METHOD(vector4, Vector4, lengthSquared);
	REGISTER_METHOD(vector4, Vector4, isUnit);
	REGISTER_METHOD(vector4, Vector4, isZero);
	REGISTER_METHOD(vector4, Vector4, normalize);
	REGISTER_METHOD(vector4, Vector4, normalized);
	REGISTER_OVERLOADED_METHOD(vector4, Vector4, operator+, "plusScalar", Vector4, const, float);
	REGISTER_OVERLOADED_METHOD(vector4, Vector4, operator+, "plusVector4", Vector4, const, const Vector4&);
	REGISTER_OVERLOADED_METHOD(vector4, Vector4, operator+=, "addScalar", Vector4&, , float);
	REGISTER_OVERLOADED_METHOD(vector4, Vector4, operator+=, "addVector4", Vector4&, , const Vector4&);
	vector4.endClass();
}


void registerMatrix(CppBindModule& module)
{
	auto matrix = module.beginClass<Matrix>("Matrix");
	matrix.addConstructor(LUA_ARGS());
	REGISTER_STATIC_METHOD(matrix, Matrix, identity);
	REGISTER_STATIC_METHOD(matrix, Matrix, zero);
	REGISTER_STATIC_OVERLOADED_METHOD(matrix, Matrix, createLookAt, "createLookAt", Matrix, , const Vector3&, const Vector3&, const Vector3&);
	REGISTER_STATIC_METHOD(matrix, Matrix, createPerspective);
	REGISTER_STATIC_METHOD(matrix, Matrix, createOrthographic);
	REGISTER_STATIC_METHOD(matrix, Matrix, createOrthographicOffCenter);
	REGISTER_STATIC_OVERLOADED_METHOD(matrix, Matrix, createBillboard, "createBillboard", Matrix, , const Vector3&, const Vector3&, const Vector3&);
	REGISTER_STATIC_OVERLOADED_METHOD(matrix, Matrix, createBillboard, "createBillboard2", Matrix, , const Vector3&, const Vector3&, const Vector3&, const Vector3&);
	REGISTER_STATIC_METHOD(matrix, Matrix, createReflection);
	REGISTER_STATIC_METHOD(matrix, Matrix, createScale);
	REGISTER_STATIC_OVERLOADED_METHOD(matrix, Matrix, createRotation, "createRotation", Matrix, , const Quaternion&);
	REGISTER_STATIC_OVERLOADED_METHOD(matrix, Matrix, createRotation, "createRotationAxisAngle", Matrix, , const Vector3&, float);
	REGISTER_STATIC_METHOD(matrix, Matrix, createRotationX);
	REGISTER_STATIC_METHOD(matrix, Matrix, createRotationY);
	REGISTER_STATIC_METHOD(matrix, Matrix, createRotationZ);
	REGISTER_STATIC_METHOD(matrix, Matrix, createTranslation);
	REGISTER_METHOD(matrix, Matrix, decompose);
	REGISTER_METHOD(matrix, Matrix, getDeterminant);
	REGISTER_METHOD(matrix, Matrix, getScale);
	REGISTER_METHOD(matrix, Matrix, getRotation);
	REGISTER_METHOD(matrix, Matrix, getTranslation);
	REGISTER_METHOD(matrix, Matrix, getUpVector);
	REGISTER_METHOD(matrix, Matrix, getDownVector);
	REGISTER_METHOD(matrix, Matrix, getLeftVector);
	REGISTER_METHOD(matrix, Matrix, getRightVector);
	REGISTER_METHOD(matrix, Matrix, getForwardVector);
	REGISTER_METHOD(matrix, Matrix, getBackVector);
	REGISTER_METHOD(matrix, Matrix, invert);
	REGISTER_METHOD(matrix, Matrix, transpose);
	REGISTER_OVERLOADED_METHOD(matrix, Matrix, rotate, "rotate", void, , const Quaternion&);
	REGISTER_OVERLOADED_METHOD(matrix, Matrix, rotate, "rotateAxisAngle", void, , const Vector3&, float);
	REGISTER_METHOD(matrix, Matrix, rotateX);
	REGISTER_METHOD(matrix, Matrix, rotateY);
	REGISTER_METHOD(matrix, Matrix, rotateZ);
	REGISTER_OVERLOADED_METHOD(matrix, Matrix, scale, "scale", void, , float);
	REGISTER_OVERLOADED_METHOD(matrix, Matrix, scale, "scale2", void, , const Vector3&);
	REGISTER_METHOD(matrix, Matrix, translate);
	REGISTER_METHOD(matrix, Matrix, setIdentity);
	REGISTER_METHOD(matrix, Matrix, setZero);
	REGISTER_METHOD(matrix, Matrix, transformPoint);
	REGISTER_OVERLOADED_METHOD(matrix, Matrix, transformDirection, "transformVector3Direction", Vector3, const, const Vector3&);
	REGISTER_OVERLOADED_METHOD(matrix, Matrix, transformDirection, "transformVector4Direction", Vector4, const, const Vector4&);
	REGISTER_OVERLOADED_METHOD(matrix, Matrix, operator+, "plusScalar", Matrix, const, float);
	REGISTER_OVERLOADED_METHOD(matrix, Matrix, operator+, "plusMatrix", Matrix, const, const Matrix&);
	REGISTER_OVERLOADED_METHOD(matrix, Matrix, operator+=, "addScalar", Matrix&, , float);
	REGISTER_OVERLOADED_METHOD(matrix, Matrix, operator+=, "addMatrix", Matrix&, , const Matrix&);
	REGISTER_OVERLOADED_METHOD(matrix, Matrix, operator-, "negative", Matrix, const, void);
	REGISTER_OVERLOADED_METHOD(matrix, Matrix, operator-, "minusScalar", Matrix, const, float);
	REGISTER_OVERLOADED_METHOD(matrix, Matrix, operator-, "minusMatrix", Matrix, const, const Matrix&);
	REGISTER_OVERLOADED_METHOD(matrix, Matrix, operator-=, "subtractScalar", Matrix&, , float);
	REGISTER_OVERLOADED_METHOD(matrix, Matrix, operator-=, "subtractMatrix", Matrix&, , const Matrix&);
	REGISTER_OVERLOADED_METHOD(matrix, Matrix, operator-, "minusScalar", Matrix, const, float);
	REGISTER_OVERLOADED_METHOD(matrix, Matrix, operator-, "minusMatrix", Matrix, const, const Matrix&);
	REGISTER_OVERLOADED_METHOD(matrix, Matrix, operator-=, "subtractScalar", Matrix&, , float);
	REGISTER_OVERLOADED_METHOD(matrix, Matrix, operator-=, "subtractMatrix", Matrix&, , const Matrix&);
	REGISTER_OVERLOADED_METHOD(matrix, Matrix, operator*, "productScalar", Matrix, const, float);
	REGISTER_OVERLOADED_METHOD(matrix, Matrix, operator*, "productMatrix", Matrix, const, const Matrix&);
	REGISTER_OVERLOADED_METHOD(matrix, Matrix, operator*=, "multScalar", Matrix&, , float);
	REGISTER_OVERLOADED_METHOD(matrix, Matrix, operator*=, "multMatrix", Matrix&, , const Matrix&);
	matrix.endClass();
}


void registerQuaternion(CppBindModule& module)
{
	auto q = module.beginClass<Quaternion>("Quaternion");
	q.addConstructor(LUA_ARGS());
	REGISTER_STATIC_METHOD(q, Quaternion, zero);
	REGISTER_STATIC_METHOD(q, Quaternion, identity);
	REGISTER_STATIC_METHOD(q, Quaternion, createFromAxisAngle);
	REGISTER_STATIC_METHOD(q, Quaternion, createFromRotationMatrix);
	REGISTER_STATIC_METHOD(q, Quaternion, lerp);
	REGISTER_STATIC_METHOD(q, Quaternion, slerp);
	REGISTER_STATIC_METHOD(q, Quaternion, squad);
	REGISTER_METHOD(q, Quaternion, isIdentity);
	REGISTER_METHOD(q, Quaternion, isZero);
	REGISTER_METHOD(q, Quaternion, conjugate);
	REGISTER_METHOD(q, Quaternion, inverse);
	REGISTER_METHOD(q, Quaternion, normalize);
	REGISTER_METHOD(q, Quaternion, normalized);
	REGISTER_METHOD(q, Quaternion, toAxisAngle);
	REGISTER_METHOD_RENAMED(q, Quaternion, operator*, "product");
	REGISTER_METHOD_RENAMED(q, Quaternion, operator*=, "mult");
	REGISTER_VARIABLE(q, Quaternion, x);
	REGISTER_VARIABLE(q, Quaternion, y);
	REGISTER_VARIABLE(q, Quaternion, z);
	REGISTER_VARIABLE(q, Quaternion, w);
	q.endClass();
}


void registerPlane(CppBindModule& module)
{
	auto p = module.beginClass<Plane>("Plane");
	p.addConstructor(LUA_ARGS(const Vector3&, float))
		.addConstant("Intersection_Intersecting", PlaneIntersection::Intersecting)
		.addConstant("Intersection_Front", PlaneIntersection::Front)
		.addConstant("Intersection_Back", PlaneIntersection::Back);
	REGISTER_STATIC_METHOD(p, Plane, intersection);
	REGISTER_METHOD(p, Plane, getNormal);
	REGISTER_METHOD(p, Plane, setNormal);
	REGISTER_METHOD(p, Plane, getDistance);
	REGISTER_METHOD(p, Plane, getDistanceToPoint);
	REGISTER_METHOD(p, Plane, setDistance);
	REGISTER_METHOD(p, Plane, isParallel);
	REGISTER_METHOD(p, Plane, transform);
	p.endClass();
}


void registerFrustum(CppBindModule& module)
{
	auto f = module.beginClass<Frustum>("Frustum");
	f.addConstructor(LUA_ARGS());
	REGISTER_METHOD(f, Frustum, getNearPlane);
	REGISTER_METHOD(f, Frustum, getFarPlane);
	REGISTER_METHOD(f, Frustum, getLeftPlane);
	REGISTER_METHOD(f, Frustum, getRightPlane);
	REGISTER_METHOD(f, Frustum, getTopPlane);
	REGISTER_METHOD(f, Frustum, getBottomPlane);
	REGISTER_METHOD(f, Frustum, getMatrix);
	REGISTER_METHOD(f, Frustum, setMatrix);
	REGISTER_METHOD(f, Frustum, getCorners);
	REGISTER_METHOD(f, Frustum, getNearCorners);
	REGISTER_METHOD(f, Frustum, getFarCorners);
	REGISTER_METHOD(f, Frustum, intersectsPoint);
	REGISTER_METHOD(f, Frustum, intersectsBoundingBox);
	REGISTER_METHOD(f, Frustum, intersectsBoundingSphere);
	REGISTER_METHOD(f, Frustum, intersectsRay);
	REGISTER_METHOD(f, Frustum, getPlaneIntersection);
	f.endClass();
}


void registerBoundingBox(CppBindModule& module)
{
	auto bb = module.beginClass<BoundingBox>("BoundingBox");
	bb.addConstructor(LUA_ARGS(const Vector3&, const Vector3&));
	REGISTER_STATIC_METHOD(bb, BoundingBox, empty);
	REGISTER_METHOD(bb, BoundingBox, getCenter);
	REGISTER_METHOD(bb, BoundingBox, getCorners);
	REGISTER_METHOD(bb, BoundingBox, intersectsBoundingBox);
	REGISTER_METHOD(bb, BoundingBox, intersectsBoundingSphere);
	REGISTER_METHOD(bb, BoundingBox, intersectsFrustum);
	REGISTER_METHOD(bb, BoundingBox, getRayIntersection);
	REGISTER_METHOD(bb, BoundingBox, getPlaneIntersection);
	REGISTER_METHOD(bb, BoundingBox, isEmpty);
	REGISTER_METHOD(bb, BoundingBox, mergeBoundingBox);
	REGISTER_METHOD(bb, BoundingBox, mergeBoundingSphere);
	REGISTER_METHOD(bb, BoundingBox, transform);
	REGISTER_VARIABLE(bb, BoundingBox, min);
	REGISTER_VARIABLE(bb, BoundingBox, max);
	bb.endClass();
}


void registerRay(CppBindModule& module)
{
	module.beginClass<Ray>("Ray")
		.addConstructor(LUA_ARGS(const Vector3&, const Vector3&))
	.endClass();
}


void registerBoundingSphere(CppBindModule& module)
{
	auto bs = module.beginClass<BoundingSphere>("BoundingSphere");
	bs.addConstructor(LUA_ARGS(const Vector3&, float));
	REGISTER_STATIC_METHOD(bs, BoundingSphere, empty);
	REGISTER_METHOD(bs, BoundingSphere, intersectsBoundingSphere);
	REGISTER_METHOD(bs, BoundingSphere, intersectsBoundingBox);
	REGISTER_METHOD(bs, BoundingSphere, intersectsFrustum);
	REGISTER_METHOD(bs, BoundingSphere, getRayIntersection);
	REGISTER_METHOD(bs, BoundingSphere, getPlaneIntersection);
	REGISTER_METHOD(bs, BoundingSphere, isEmpty);
	REGISTER_METHOD(bs, BoundingSphere, mergeBoundingBox);
	REGISTER_METHOD(bs, BoundingSphere, mergeBoundingSphere);
	REGISTER_METHOD(bs, BoundingSphere, transform);
	REGISTER_VARIABLE(bs, BoundingSphere, center);
	REGISTER_VARIABLE(bs, BoundingSphere, radius);
	bs.endClass();
}


void registerDevice(CppBindModule& module)
{
	module
		.addConstant("KeyCode_A", KeyCode::A)
		.addConstant("KeyCode_B", KeyCode::B)
		.addConstant("KeyCode_C", KeyCode::C)
		.addConstant("KeyCode_D", KeyCode::D)
		.addConstant("KeyCode_E", KeyCode::E)
		.addConstant("KeyCode_F", KeyCode::F)
		.addConstant("KeyCode_G", KeyCode::G)
		.addConstant("KeyCode_H", KeyCode::H)
		.addConstant("KeyCode_I", KeyCode::I)
		.addConstant("KeyCode_J", KeyCode::J)
		.addConstant("KeyCode_K", KeyCode::K)
		.addConstant("KeyCode_L", KeyCode::L)
		.addConstant("KeyCode_M", KeyCode::M)
		.addConstant("KeyCode_N", KeyCode::N)
		.addConstant("KeyCode_O", KeyCode::O)
		.addConstant("KeyCode_P", KeyCode::P)
		.addConstant("KeyCode_Q", KeyCode::Q)
		.addConstant("KeyCode_R", KeyCode::R)
		.addConstant("KeyCode_S", KeyCode::S)
		.addConstant("KeyCode_T", KeyCode::T)
		.addConstant("KeyCode_U", KeyCode::U)
		.addConstant("KeyCode_V", KeyCode::V)
		.addConstant("KeyCode_W", KeyCode::W)
		.addConstant("KeyCode_X", KeyCode::X)
		.addConstant("KeyCode_Y", KeyCode::Y)
		.addConstant("KeyCode_Z", KeyCode::Z)
		.addConstant("KeyCode_LeftArrow", KeyCode::LeftArrow)
		.addConstant("KeyCode_RightArrow", KeyCode::RightArrow)
		.addConstant("KeyCode_UpArrow", KeyCode::UpArrow)
		.addConstant("KeyCode_DownArrow", KeyCode::DownArrow)
		.addConstant("KeyCode_Escape", KeyCode::Escape);

	module
		.addConstant("MouseButton_Left", MouseButton::Left)
		.addConstant("MouseButton_Middle", MouseButton::Middle)
		.addConstant("MouseButton_Right", MouseButton::Right);

	auto device = module.beginClass<Device>("Device");
	REGISTER_METHOD(device, Device, getWindowTitle);
	REGISTER_METHOD(device, Device, setWindowTitle);
	REGISTER_METHOD(device, Device, setCursorCaptured);
	REGISTER_METHOD(device, Device, getCanvasSize);
	REGISTER_METHOD(device, Device, getLifetime);
	REGISTER_METHOD(device, Device, isKeyPressed);
	REGISTER_METHOD(device, Device, isKeyReleased);
	REGISTER_METHOD(device, Device, getMouseMotion);
	REGISTER_METHOD(device, Device, isMouseButtonDown);
	REGISTER_METHOD(device, Device, isMouseButtonReleased);
	REGISTER_METHOD(device, Device, getTimeDelta);
	REGISTER_METHOD(device, Device, requestShutdown);
	REGISTER_METHOD(device, Device, shutdownRequested);
	device.endClass();
}


void registerEffect(CppBindModule& module)
{
	auto effect = module.beginClass<Effect>("Effect");
	REGISTER_METHOD(effect, Effect, findVariable);
	effect.endClass();
}


void registerTexture(CppBindModule& module)
{
	module
		.addConstant("ColorFormat_RGB", ColorFormat::RGB)
		.addConstant("ColorFormat_RGBA", ColorFormat::RGBA)
		.addConstant("TextureWrapMode_Clamp", TextureWrapMode::Clamp)
		.addConstant("TextureWrapMode_Repeat", TextureWrapMode::Repeat)
		.addConstant("TextureFilter_Nearest", TextureFilter::Nearest)
		.addConstant("TextureFilter_Linear", TextureFilter::Linear)
		.addConstant("TextureFilter_LinearMipmapLinear", TextureFilter::LinearMipmapLinear)
		.addConstant("TextureFilter_LinearMipmapNearest", TextureFilter::LinearMipmapNearest)
		.addConstant("TextureFilter_NearestMipmapLinear", TextureFilter::NearestMipmapLinear)
		.addConstant("TextureFilter_NearestMipmapNearest", TextureFilter::NearestMipmapNearest);

	module.beginClass<Texture>("Texture") // TODO not sure if empty registration needed
	.endClass();

	auto tex2d = module.beginExtendClass<Texture2D, Texture>("Texture2D");
	REGISTER_METHOD(tex2d, Texture2D, setData);
	REGISTER_METHOD(tex2d, Texture2D, generateMipmaps);
	REGISTER_METHOD(tex2d, Texture2D, getSize);
	REGISTER_METHOD(tex2d, Texture2D, getVerticalWrapMode);
	REGISTER_METHOD(tex2d, Texture2D, getHorizontalWrapMode);
	REGISTER_METHOD(tex2d, Texture2D, setWrapMode);
	REGISTER_METHOD(tex2d, Texture2D, getMinFilter);
	REGISTER_METHOD(tex2d, Texture2D, getMagFilter);
	REGISTER_METHOD(tex2d, Texture2D, setFilterMode);
	REGISTER_METHOD(tex2d, Texture2D, getAnisotropyLevel);
	REGISTER_METHOD(tex2d, Texture2D, setAnisotropyLevel);
	tex2d.endClass();
}


void registerMaterial(CppBindModule& module)
{
	auto m = module.beginClass<Material>("Material");
	REGISTER_METHOD(m, Material, getParameter);
	REGISTER_METHOD(m, Material, getEffect);
	m.endClass();

	module
		.addConstant("AutoBinding_None", AutoBinding::None)
		.addConstant("AutoBinding_CameraWorldPosition", AutoBinding::CameraWorldPosition)
		.addConstant("AutoBinding_InverseTransposedWorldMatrix", AutoBinding::InverseTransposedWorldMatrix)
		.addConstant("AutoBinding_InverseTransposedWorldViewMatrix", AutoBinding::InverseTransposedWorldViewMatrix)
		.addConstant("AutoBinding_ProjectionMatrix", AutoBinding::ProjectionMatrix)
		.addConstant("AutoBinding_ViewMatrix", AutoBinding::ViewMatrix)
		.addConstant("AutoBinding_ViewProjectionMatrix", AutoBinding::ViewProjectionMatrix)
		.addConstant("AutoBinding_WorldMatrix", AutoBinding::WorldMatrix)
		.addConstant("AutoBinding_WorldViewProjectionMatrix", AutoBinding::WorldViewProjectionMatrix);

	auto mp = module.beginClass<MaterialParameter>("MaterialParameter");
	REGISTER_METHOD(mp, MaterialParameter, setFloat);
	REGISTER_METHOD(mp, MaterialParameter, setFloatArray);
	REGISTER_METHOD(mp, MaterialParameter, setInt);
	REGISTER_METHOD(mp, MaterialParameter, setIntArray);
	REGISTER_METHOD(mp, MaterialParameter, setVector2);
	REGISTER_METHOD(mp, MaterialParameter, setVector2Array);
	REGISTER_METHOD(mp, MaterialParameter, setVector3);
	REGISTER_METHOD(mp, MaterialParameter, setVector3Array);
	REGISTER_METHOD(mp, MaterialParameter, setVector4);
	REGISTER_METHOD(mp, MaterialParameter, setVector4Array);
	REGISTER_METHOD(mp, MaterialParameter, setMatrix);
	REGISTER_METHOD(mp, MaterialParameter, setMatrixArray);
	REGISTER_METHOD(mp, MaterialParameter, setTexture);
	REGISTER_METHOD(mp, MaterialParameter, setTextureArray);
	REGISTER_METHOD(mp, MaterialParameter, bindValue);
	mp.endClass();
}


void registerNode(CppBindModule& module)
{
	auto node = module.beginClass<Node>("Node");
	REGISTER_METHOD(node, Node, getScene);
	REGISTER_METHOD(node, Node, getId);
	REGISTER_METHOD(node, Node, removeAllComponents);
	node.addFunction("findComponent", LuaScriptComponent::getFindComponentFunc(module.state()))
		.addFunction("findStandardComponent", &LuaScriptComponent::findStandardComponent)
		.addFunction("addComponent", &LuaScriptComponent::addComponent)
		.addFunction("addStandardComponent", &LuaScriptComponent::addStandardComponent)
		.addFunction("removeComponent", &LuaScriptComponent::removeComponent)
	.endClass();
}


void registerComponent(CppBindModule& module)
{
	module.beginClass<Component>("Component")
		// TODO
	.endClass();
}


void registerRenderTarget(CppBindModule& module)
{
	module.beginClass<RenderTarget>("RenderTarget")
		// TODO
	.endClass();
}


void registerMesh(CppBindModule& module)
{
	auto mesh = module.beginClass<Mesh>("Mesh");
	REGISTER_METHOD(mesh, Mesh, setIndices);
	REGISTER_METHOD(mesh, Mesh, setNormals);
	REGISTER_METHOD(mesh, Mesh, setUVs);
	REGISTER_METHOD(mesh, Mesh, setVertices);
	mesh.endClass();
}


void registerCamera(CppBindModule& module)
{
	auto camera = module.beginExtendClass<Camera, Component>("Camera");
	REGISTER_METHOD(camera, Camera, getRenderTarget);
	REGISTER_METHOD(camera, Camera, setRenderTarget);
	REGISTER_METHOD(camera, Camera, setClearColor);
	REGISTER_METHOD(camera, Camera, getViewport);
	REGISTER_METHOD(camera, Camera, setViewport);
	REGISTER_METHOD(camera, Camera, resetViewport);
	REGISTER_METHOD(camera, Camera, setPerspective);
	REGISTER_METHOD(camera, Camera, isPerspective);
	REGISTER_METHOD(camera, Camera, getNear);
	REGISTER_METHOD(camera, Camera, getFar);
	REGISTER_METHOD(camera, Camera, getFOV);
	REGISTER_METHOD(camera, Camera, getWidth);
	REGISTER_METHOD(camera, Camera, getHeight);
	REGISTER_METHOD(camera, Camera, getAspectRatio);
	REGISTER_METHOD(camera, Camera, setNear);
	REGISTER_METHOD(camera, Camera, setFar);
	REGISTER_METHOD(camera, Camera, setFOV);
	REGISTER_METHOD(camera, Camera, setWidth);
	REGISTER_METHOD(camera, Camera, setHeight);
	REGISTER_METHOD(camera, Camera, setAspectRatio);
	camera.endClass();
}


void registerTransform(CppBindModule& module)
{
	module
		.addConstant("TransformSpace_Parent", TransformSpace::Parent)
		.addConstant("TransformSpace_Self", TransformSpace::Self)
		.addConstant("TransformSpace_World", TransformSpace::World);

	auto transform = module.beginExtendClass<Transform, Component>("Transform");
	REGISTER_METHOD(transform, Transform, getParent);
	REGISTER_METHOD(transform, Transform, setParent);
	REGISTER_METHOD(transform, Transform, getChild);
	REGISTER_METHOD(transform, Transform, getChildrenCount);
	REGISTER_METHOD(transform, Transform, removeChildren);
	REGISTER_METHOD(transform, Transform, getWorldScale);
	REGISTER_METHOD(transform, Transform, getLocalScale);
	REGISTER_METHOD(transform, Transform, getWorldRotation);
	REGISTER_METHOD(transform, Transform, getLocalRotation);
	REGISTER_METHOD(transform, Transform, getWorldPosition);
	REGISTER_METHOD(transform, Transform, getLocalPosition);
	REGISTER_METHOD(transform, Transform, getLocalUp);
	REGISTER_METHOD(transform, Transform, getLocalDown);
	REGISTER_METHOD(transform, Transform, getLocalLeft);
	REGISTER_METHOD(transform, Transform, getLocalRight);
	REGISTER_METHOD(transform, Transform, getLocalForward);
	REGISTER_METHOD(transform, Transform, getLocalBack);
	REGISTER_METHOD(transform, Transform, translateLocal);
	REGISTER_OVERLOADED_METHOD(transform, Transform, rotate, "rotate", void, , const Quaternion&, TransformSpace);
	REGISTER_OVERLOADED_METHOD(transform, Transform, rotate, "rotateAxisAngle", void, , const Vector3&, float, TransformSpace);
	REGISTER_METHOD(transform, Transform, scaleLocal);
	REGISTER_METHOD(transform, Transform, setLocalPosition);
	REGISTER_OVERLOADED_METHOD(transform, Transform, setLocalRotation, "setLocalRotation", void, , const Quaternion&);
	REGISTER_OVERLOADED_METHOD(transform, Transform, setLocalRotation, "setLocalRotationAxisAngle", void, , const Vector3&, float);
	REGISTER_METHOD(transform, Transform, setLocalScale);
	REGISTER_METHOD(transform, Transform, lookAt);
	REGISTER_METHOD(transform, Transform, getMatrix);
	REGISTER_METHOD(transform, Transform, getWorldMatrix);
	REGISTER_METHOD(transform, Transform, getWorldViewMatrix);
	REGISTER_METHOD(transform, Transform, getWorldViewProjectionMatrix);
	REGISTER_METHOD(transform, Transform, getInverseTransposedWorldMatrix);
	REGISTER_METHOD(transform, Transform, getInverseTransposedWorldViewMatrix);
	REGISTER_METHOD(transform, Transform, transformPoint);
	REGISTER_METHOD(transform, Transform, transformDirection);
	transform.endClass();
}


void registerScene(CppBindModule& module)
{
	module.beginClass<Scene>("Scene")
		.addFunction("createNode", &Scene::createNode)
	.endClass();
}


void registerFileSystem(CppBindModule& module)
{
	module.beginClass<FileSystem>("FileSystem")
		// TODO
	.endClass();
}


void registerEngine(CppBindModule& module)
{
	module
		.addConstant("EngineMode_Stub", EngineMode::Stub)
		.addConstant("EngineMode_OpenGL", EngineMode::OpenGL);

	module.beginClass<EngineCreationArgs>("EngineCreationArgs")
		.addConstructor(LUA_ARGS(_opt<EngineMode>, _opt<int>, _opt<int>, _opt<int>, _opt<int>, _opt<bool>, _opt<std::string>))
		.addVariable("mode", &EngineCreationArgs::mode, true)
		.addVariable("bits", &EngineCreationArgs::bits, true)
		.addVariable("canvasHeight", &EngineCreationArgs::canvasHeight, true)
		.addVariable("canvasWidth", &EngineCreationArgs::canvasWidth, true)
		.addVariable("depth", &EngineCreationArgs::depth, true)
		.addVariable("fullScreen", &EngineCreationArgs::fullScreen, true)
		.addVariable("windowTitle", &EngineCreationArgs::windowTitle, true)
	.endClass();

	module.beginClass<Engine>("Engine")
		.addStaticFunction("create", &Engine::create)
		.addFunction("getDevice", &Engine::getDevice)
		.addFunction("getScene", &Engine::getScene)
		.addFunction("getResourceManager", &Engine::getResourceManager)
		.addFunction("getFileSystem", &Engine::getFileSystem)
		.addFunction("setCallback", &LuaEngineCallback::setCallback)
		.addFunction("getMode", &Engine::getMode)
		.addFunction("run", &Engine::run)
	.endClass();
}


void registerResourceManager(CppBindModule& module)
{
	auto mgr = module.beginClass<ResourceManager>("ResourceManager");
	REGISTER_METHOD(mgr, ResourceManager, findEffect);
	REGISTER_METHOD(mgr, ResourceManager, findTexture);
	REGISTER_METHOD(mgr, ResourceManager, findMaterial);
	REGISTER_METHOD(mgr, ResourceManager, findMesh);
	REGISTER_METHOD(mgr, ResourceManager, findModel);
	REGISTER_METHOD(mgr, ResourceManager, findRenderTarget);
	REGISTER_METHOD2(mgr, ResourceManager, getOrCreateEffect, LUA_ARGS(const std::string&, const std::string&, _opt<const std::string&>));
	REGISTER_METHOD2(mgr, ResourceManager, getOrCreateTexture2D, LUA_ARGS(_opt<const std::string&>));
	REGISTER_METHOD2(mgr, ResourceManager, getOrCreateMaterial, LUA_ARGS(shared<Effect>, _opt<const std::string&>));
	REGISTER_METHOD2(mgr, ResourceManager, getOrCreateMesh, LUA_ARGS(_opt<const std::string&>));
	REGISTER_METHOD2(mgr, ResourceManager, getOrCreateModel, LUA_ARGS(_opt<const std::string&>));
	REGISTER_METHOD2(mgr, ResourceManager, getOrCreateRenderTarget, LUA_ARGS(_opt<const std::string&>));
	REGISTER_METHOD(mgr, ResourceManager, getOrLoadTexture);
	REGISTER_METHOD(mgr, ResourceManager, getOrLoadModel);
	REGISTER_OVERLOADED_METHOD(mgr, ResourceManager, cleanUnusedResources, "cleanUnusedResources", void,,void);
	mgr.endClass();
}


void registerModel(CppBindModule& module)
{
	auto m = module.beginClass<Model>("Model");
	REGISTER_METHOD(m, Model, addMesh);
	REGISTER_METHOD(m, Model, getMesh);
	REGISTER_METHOD(m, Model, getMeshCount);
	REGISTER_METHOD(m, Model, removeMesh);
	m.endClass();
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
	registerEffect(module);
	registerModel(module);
	registerTexture(module);
	registerResourceManager(module);
	registerNode(module);
	registerComponent(module);
	registerTransform(module);
	registerRenderTarget(module);
	registerCamera(module);
	registerMaterial(module);
	registerMesh(module);
	registerScene(module);
	registerFileSystem(module);
	registerEngine(module);
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