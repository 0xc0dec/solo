#include "SoloBase.h"
#include "SoloLuaScriptManager.h"
#include "SoloLuaScriptComponent.h"
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
#include "SoloCubeTexture.h"
#include "SoloMaterial.h"
#include "SoloMaterialParameter.h"
#include "SoloMesh.h"
#include "SoloLuaMesh.h"
#include "SoloRenderContext.h"
#include "SoloMeshRenderer.h"
#include "SoloSkyboxRenderer.h"
#include "SoloSpectator.h"
#include "SoloSurfaceRenderer.h"
#include "SoloLuaSurfaceRenderer.h"
#include "SoloLogger.h"
#include <map>


#define REGISTER_VARIABLE(binding, klass, name) binding.addVariable(#name, &klass::name, true)
#define REGISTER_METHOD(binding, klass, name) binding.addFunction(#name, &klass::name)
#define REGISTER_METHOD2(binding, klass, name, argsSpec) binding.addFunction(#name, &klass::name, argsSpec)
#define REGISTER_METHOD_RENAMED(binding, klass, name, nameStr) binding.addFunction(nameStr, &klass::name)
#define REGISTER_OVERLOADED_METHOD(binding, klass, name, nameStr, resultType, modifier, ...) \
	binding.addFunction(nameStr, static_cast<resultType(klass::*)(__VA_ARGS__)modifier>(&klass::name))
#define REGISTER_STATIC_METHOD(binding, klass, name) binding.addStaticFunction(#name, &klass::name)
#define REGISTER_STATIC_OVERLOADED_METHOD(binding, klass, name, nameStr, resultType, modifier, ...) \
	binding.addStaticFunction(nameStr, static_cast<resultType(*)(__VA_ARGS__)modifier>(&klass::name))
#define REGISTER_MODULE_CONSTANT(module, holder, constant) module.addConstant(#constant, holder::constant)


namespace LuaIntf
{
    LUA_USING_SHARED_PTR_TYPE(std::shared_ptr)
    LUA_USING_LIST_TYPE(std::vector)
    LUA_USING_MAP_TYPE(std::map) // TODO unordered_map
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


void LuaScriptManager::execute(const std::string& code)
{
    lua.doString(code.c_str());
}


void LuaScriptManager::executeFile(const std::string& path)
{
    lua.doFile(path.c_str());
}


void LuaScriptManager::registerApi()
{
    auto module = LuaBinding(lua).beginModule("solo");

    // Vector2
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
    REGISTER_OVERLOADED_METHOD(vector2, Vector2, operator+=, "addScalar", Vector2&, , float);
    REGISTER_OVERLOADED_METHOD(vector2, Vector2, operator+=, "addVector2", Vector2&, , const Vector2&);
    REGISTER_OVERLOADED_METHOD(vector2, Vector2, operator*, "productScalar", Vector2, const, float);
    REGISTER_OVERLOADED_METHOD(vector2, Vector2, operator*=, "multScalar", Vector2&, , float);
    vector2.endClass();

    // Vector3
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
    REGISTER_OVERLOADED_METHOD(vector3, Vector3, operator+=, "addScalar", Vector3&, , float);
    REGISTER_OVERLOADED_METHOD(vector3, Vector3, operator+, "plusVector3", Vector3, const, const Vector3&);
    REGISTER_OVERLOADED_METHOD(vector3, Vector3, operator+=, "addVector3", Vector3&, , const Vector3&);
    REGISTER_OVERLOADED_METHOD(vector3, Vector3, operator+, "multScalar", Vector3, const, float);
    REGISTER_OVERLOADED_METHOD(vector3, Vector3, operator*, "productScalar", Vector3, const, float);
    REGISTER_OVERLOADED_METHOD(vector3, Vector3, operator*=, "multScalar", Vector3&, , float);
    vector3.endClass();

    // Vector4
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
    REGISTER_OVERLOADED_METHOD(vector4, Vector4, operator*, "productScalar", Vector4, const, float);
    REGISTER_OVERLOADED_METHOD(vector4, Vector4, operator*=, "multScalar", Vector4&, , float);
    vector4.endClass();

    // Quaternion
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

    // Matrix
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

    // Ray
    module.beginClass<Ray>("Ray")
    .addConstructor(LUA_ARGS(const Vector3&, const Vector3&))
    .endClass();

    // BoundingBox
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

    // BoundingSphere
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

    // PlaneIntersection
    auto planeIntersection = module.beginModule("PlaneIntersection");
    REGISTER_MODULE_CONSTANT(planeIntersection, PlaneIntersection, Intersecting);
    REGISTER_MODULE_CONSTANT(planeIntersection, PlaneIntersection, Front);
    REGISTER_MODULE_CONSTANT(planeIntersection, PlaneIntersection, Back);
    planeIntersection.endModule();

    // Plane
    auto p = module.beginClass<Plane>("Plane");
    p.addConstructor(LUA_ARGS(const Vector3&, float));
    REGISTER_STATIC_METHOD(p, Plane, intersection);
    REGISTER_METHOD(p, Plane, getNormal);
    REGISTER_METHOD(p, Plane, setNormal);
    REGISTER_METHOD(p, Plane, getDistance);
    REGISTER_METHOD(p, Plane, getDistanceToPoint);
    REGISTER_METHOD(p, Plane, setDistance);
    REGISTER_METHOD(p, Plane, isParallel);
    REGISTER_METHOD(p, Plane, transform);
    p.endClass();

    // Frustum
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

    // EffectPrefab
    auto effectPrefab = module.beginModule("EffectPrefab");
    REGISTER_MODULE_CONSTANT(effectPrefab, EffectPrefab, Skybox);
    effectPrefab.endModule();

    // EffectVariable
    auto var = module.beginClass<EffectVariable>("EffectVariable");
    REGISTER_METHOD(var, EffectVariable, getName);
    REGISTER_METHOD(var, EffectVariable, setFloat);
    REGISTER_METHOD(var, EffectVariable, setFloatArray);
    REGISTER_METHOD(var, EffectVariable, setInt);
    REGISTER_METHOD(var, EffectVariable, setIntArray);
    REGISTER_METHOD(var, EffectVariable, setVector2);
    REGISTER_METHOD(var, EffectVariable, setVector2Array);
    REGISTER_METHOD(var, EffectVariable, setVector3);
    REGISTER_METHOD(var, EffectVariable, setVector3Array);
    REGISTER_METHOD(var, EffectVariable, setVector4);
    REGISTER_METHOD(var, EffectVariable, setVector4Array);
    REGISTER_METHOD(var, EffectVariable, setMatrix);
    REGISTER_METHOD(var, EffectVariable, setMatrixArray);
    REGISTER_METHOD(var, EffectVariable, setTexture);
    REGISTER_METHOD(var, EffectVariable, setTextureArray);
    var.endClass();

    // Effect
    auto effect = module.beginClass<Effect>("Effect");
    REGISTER_METHOD(effect, Effect, findVariable);
    effect.endClass();

    // MeshPrefab
    auto meshPrefab = module.beginModule("MeshPrefab");
    REGISTER_MODULE_CONSTANT(meshPrefab, MeshPrefab, Cube);
    REGISTER_MODULE_CONSTANT(meshPrefab, MeshPrefab, Quad);
    meshPrefab.endModule();

    // Mesh
    auto mesh = module.beginClass<Mesh>("Mesh");
    REGISTER_METHOD(mesh, LuaMesh, resetData);
    REGISTER_METHOD(mesh, LuaMesh, updateData);
    REGISTER_METHOD(mesh, LuaMesh, resetIndexData);
    REGISTER_METHOD(mesh, LuaMesh, updateIndexData);
    REGISTER_METHOD(mesh, LuaMesh, addIndex);
    REGISTER_METHOD(mesh, Mesh, removeIndex);
    REGISTER_METHOD(mesh, Mesh, getIndexCount);
    REGISTER_METHOD(mesh, Mesh, getIndexPrimitiveType);
    REGISTER_METHOD(mesh, Mesh, setIndexPrimitiveType);
    REGISTER_METHOD(mesh, Mesh, getVertexFormat);
    REGISTER_METHOD(mesh, Mesh, setPrimitiveType);
    REGISTER_METHOD(mesh, Mesh, getPrimitiveType);
    mesh.endClass();

    // MeshPrimitiveType
    auto meshPrimitiveType = module.beginModule("MeshPrimitiveType");
    REGISTER_MODULE_CONSTANT(meshPrimitiveType, MeshPrimitiveType, Lines);
    REGISTER_MODULE_CONSTANT(meshPrimitiveType, MeshPrimitiveType, LineStrip);
    REGISTER_MODULE_CONSTANT(meshPrimitiveType, MeshPrimitiveType, Points);
    REGISTER_MODULE_CONSTANT(meshPrimitiveType, MeshPrimitiveType, TriangleStrip);
    REGISTER_MODULE_CONSTANT(meshPrimitiveType, MeshPrimitiveType, Triangles);
    meshPrimitiveType.endModule();

    // MeshIndexFormat
    auto meshIndexFormat = module.beginModule("MeshIndexFormat");
    REGISTER_MODULE_CONSTANT(meshIndexFormat, MeshIndexFormat, UnsignedByte);
    REGISTER_MODULE_CONSTANT(meshIndexFormat, MeshIndexFormat, UnsignedShort);
    REGISTER_MODULE_CONSTANT(meshIndexFormat, MeshIndexFormat, UnsignedInt);
    meshIndexFormat.endModule();

    // VertexFormatElementSemantics
    auto vertexFormatElementSemantics = module.beginModule("VertexFormatElementSemantics");
    REGISTER_MODULE_CONSTANT(vertexFormatElementSemantics, VertexFormatElementSemantics, Position);
    REGISTER_MODULE_CONSTANT(vertexFormatElementSemantics, VertexFormatElementSemantics, Normal);
    REGISTER_MODULE_CONSTANT(vertexFormatElementSemantics, VertexFormatElementSemantics, Color);
    REGISTER_MODULE_CONSTANT(vertexFormatElementSemantics, VertexFormatElementSemantics, Tangent);
    REGISTER_MODULE_CONSTANT(vertexFormatElementSemantics, VertexFormatElementSemantics, Binormal);
    REGISTER_MODULE_CONSTANT(vertexFormatElementSemantics, VertexFormatElementSemantics, TexCoord0);
    REGISTER_MODULE_CONSTANT(vertexFormatElementSemantics, VertexFormatElementSemantics, TexCoord1);
    REGISTER_MODULE_CONSTANT(vertexFormatElementSemantics, VertexFormatElementSemantics, TexCoord2);
    REGISTER_MODULE_CONSTANT(vertexFormatElementSemantics, VertexFormatElementSemantics, TexCoord3);
    REGISTER_MODULE_CONSTANT(vertexFormatElementSemantics, VertexFormatElementSemantics, TexCoord4);
    REGISTER_MODULE_CONSTANT(vertexFormatElementSemantics, VertexFormatElementSemantics, TexCoord5);
    REGISTER_MODULE_CONSTANT(vertexFormatElementSemantics, VertexFormatElementSemantics, TexCoord6);
    REGISTER_MODULE_CONSTANT(vertexFormatElementSemantics, VertexFormatElementSemantics, TexCoord7);
    vertexFormatElementSemantics.endModule();

    // VertexFormatElement
    auto vertexFormatElement = module.beginClass<VertexFormatElement>("VertexFormatElement");
    vertexFormatElement.addConstructor(LUA_ARGS(VertexFormatElementSemantics, int, int));
    REGISTER_VARIABLE(vertexFormatElement, VertexFormatElement, size);
    REGISTER_VARIABLE(vertexFormatElement, VertexFormatElement, semantics);
    REGISTER_VARIABLE(vertexFormatElement, VertexFormatElement, storageId);
    vertexFormatElement.endClass();

    // VertexFormat
    auto vertexFormat = module.beginClass<VertexFormat>("VertexFormat");
    vertexFormat.addConstructor(LUA_ARGS(const std::vector<VertexFormatElement>&));
    REGISTER_METHOD(vertexFormat, VertexFormat, getVertexSize);
    REGISTER_METHOD(vertexFormat, VertexFormat, getElement);
    REGISTER_METHOD(vertexFormat, VertexFormat, getElementCount);
    vertexFormat.endClass();

    // ColorFormat
    auto colorFormat = module.beginModule("ColorFormat");
    REGISTER_MODULE_CONSTANT(colorFormat, ColorFormat, RGB);
    REGISTER_MODULE_CONSTANT(colorFormat, ColorFormat, RGBA);
    colorFormat.endModule();

    // BitFlags
    auto flags = module.beginClass<BitFlags>("BitFlags");
    flags.addConstructor(LUA_ARGS());
    REGISTER_METHOD(flags, BitFlags, clear);
    REGISTER_METHOD(flags, BitFlags, add);
    REGISTER_METHOD(flags, BitFlags, checkAndRemove);
    REGISTER_METHOD(flags, BitFlags, getRaw);
    REGISTER_METHOD(flags, BitFlags, isEmpty);
    REGISTER_METHOD(flags, BitFlags, isSet);
    REGISTER_METHOD(flags, BitFlags, remove);
    REGISTER_METHOD(flags, BitFlags, set);
    flags.endClass();

    // TextureWrapping
    auto textureWrapping = module.beginModule("TextureWrapping");
    REGISTER_MODULE_CONSTANT(textureWrapping, TextureWrapping, Clamp);
    REGISTER_MODULE_CONSTANT(textureWrapping, TextureWrapping, Repeat);
    textureWrapping.endModule();

    // TextureFiltering
    auto textureFiltering = module.beginModule("TextureFiltering");
    REGISTER_MODULE_CONSTANT(textureFiltering, TextureFiltering, Nearest);
    REGISTER_MODULE_CONSTANT(textureFiltering, TextureFiltering, Linear);
    REGISTER_MODULE_CONSTANT(textureFiltering, TextureFiltering, LinearMipmapLinear);
    REGISTER_MODULE_CONSTANT(textureFiltering, TextureFiltering, LinearMipmapNearest);
    REGISTER_MODULE_CONSTANT(textureFiltering, TextureFiltering, NearestMipmapLinear);
    REGISTER_MODULE_CONSTANT(textureFiltering, TextureFiltering, NearestMipmapNearest);
    textureFiltering.endModule();

    // Texture
    auto tex = module.beginClass<Texture>("Texture");
    REGISTER_METHOD(tex, Texture, getHorizontalWrapping);
    REGISTER_METHOD(tex, Texture, setHorizontalWrapping);
    REGISTER_METHOD(tex, Texture, getVerticalWrapping);
    REGISTER_METHOD(tex, Texture, setVerticalWrapping);
    REGISTER_METHOD(tex, Texture, setWrapping);
    REGISTER_METHOD(tex, Texture, getMinFiltering);
    REGISTER_METHOD(tex, Texture, setMinFiltering);
    REGISTER_METHOD(tex, Texture, getMagFiltering);
    REGISTER_METHOD(tex, Texture, setMagFiltering);
    REGISTER_METHOD(tex, Texture, setFiltering);
    REGISTER_METHOD(tex, Texture, getAnisotropyLevel);
    REGISTER_METHOD(tex, Texture, setAnisotropyLevel);
    tex.endClass();

    // Texture2D
    auto tex2d = module.beginExtendClass<Texture2D, Texture>("Texture2D");
    REGISTER_METHOD(tex2d, Texture2D, setData);
    REGISTER_METHOD(tex2d, Texture2D, generateMipmaps);
    REGISTER_METHOD(tex2d, Texture2D, getSize);
    tex2d.endClass();

    // CubeTextureFace
    auto cubeTextureFace = module.beginModule("CubeTextureFace");
    REGISTER_MODULE_CONSTANT(cubeTextureFace, CubeTextureFace, Front);
    REGISTER_MODULE_CONSTANT(cubeTextureFace, CubeTextureFace, Back);
    REGISTER_MODULE_CONSTANT(cubeTextureFace, CubeTextureFace, Left);
    REGISTER_MODULE_CONSTANT(cubeTextureFace, CubeTextureFace, Right);
    REGISTER_MODULE_CONSTANT(cubeTextureFace, CubeTextureFace, Top);
    REGISTER_MODULE_CONSTANT(cubeTextureFace, CubeTextureFace, Bottom);
    cubeTextureFace.endModule();

    // CubeTexture
    auto texCube = module.beginExtendClass<CubeTexture, Texture>("CubeTexture");
    REGISTER_METHOD(texCube, CubeTexture, getDepthWrapping);
    REGISTER_METHOD(texCube, CubeTexture, setDepthWrapping);
    REGISTER_METHOD(texCube, CubeTexture, generateMipmaps);
    REGISTER_METHOD(texCube, CubeTexture, setData);
    texCube.endClass();

    // RenderTarget
    auto rt = module.beginClass<RenderTarget>("RenderTarget");
    REGISTER_METHOD(rt, RenderTarget, getColorAttachmentSize);
    REGISTER_METHOD(rt, RenderTarget, getColorAttachmentCount);
    REGISTER_METHOD(rt, RenderTarget, getColorAttachment);
    REGISTER_METHOD(rt, RenderTarget, setColorAttachment);
    rt.endClass();

    // Node
    auto node = module.beginClass<Node>("Node");
    REGISTER_METHOD(node, Node, getScene);
    REGISTER_METHOD(node, Node, getId);
    REGISTER_METHOD(node, LuaScriptComponent, addScript);
    REGISTER_METHOD(node, LuaScriptComponent, removeScript);
    REGISTER_METHOD(node, LuaScriptComponent, findComponent);
    REGISTER_METHOD(node, LuaScriptComponent, addComponent);
    REGISTER_METHOD(node, LuaScriptComponent, removeComponent);
    node.endClass();

    // Component
    auto component = module.beginClass<Component>("Component");
    REGISTER_METHOD(component, Component, getRenderQueue);
    REGISTER_METHOD(component, Component, setRenderQueue);
    REGISTER_METHOD(component, Component, getTags);
    REGISTER_METHOD(component, Component, getNode);
    component.endClass();

    // TransformSpace
    auto transformSpace = module.beginModule("TransformSpace");
    REGISTER_MODULE_CONSTANT(transformSpace, TransformSpace, Parent);
    REGISTER_MODULE_CONSTANT(transformSpace, TransformSpace, Self);
    REGISTER_MODULE_CONSTANT(transformSpace, TransformSpace, World);
    transformSpace.endModule();

    // MeshRenderer
    auto mr = module.beginExtendClass<MeshRenderer, Component>("MeshRenderer");
    REGISTER_METHOD(mr, MeshRenderer, findMaterial);
    REGISTER_METHOD(mr, MeshRenderer, setMaterial);
    REGISTER_METHOD(mr, MeshRenderer, getMaterialCount);
    REGISTER_METHOD(mr, MeshRenderer, getMesh);
    REGISTER_METHOD(mr, MeshRenderer, setMesh);
    mr.endClass();

    // SkyboxRenderer
    auto sr = module.beginExtendClass<SkyboxRenderer, Component>("SkyboxRenderer");
    REGISTER_METHOD(sr, SkyboxRenderer, getTexture);
    REGISTER_METHOD(sr, SkyboxRenderer, setTexture);
    sr.endClass();

    // Transform
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

    // Camera
    auto camera = module.beginExtendClass<Camera, Component>("Camera");
    REGISTER_METHOD(camera, Camera, getRenderTarget);
    REGISTER_METHOD(camera, Camera, setRenderTarget);
    REGISTER_METHOD(camera, Camera, getClearColor);
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
    REGISTER_METHOD(camera, Camera, getRenderTags);
    camera.endClass();

    // Spectator
    auto spectator = module.beginExtendClass<Spectator, Component>("Spectator");
    REGISTER_METHOD(spectator, Spectator, getHorizontalRotationSpeed);
    REGISTER_METHOD(spectator, Spectator, setHorizontalRotationSpeed);
    REGISTER_METHOD(spectator, Spectator, getVerticalRotationSpeed);
    REGISTER_METHOD(spectator, Spectator, setVerticalRotationSpeed);
    REGISTER_METHOD(spectator, Spectator, getMovementSpeed);
    REGISTER_METHOD(spectator, Spectator, setMovementSpeed);
    spectator.endClass();

    // PolygonFace
    auto polygonFace = module.beginModule("PolygonFace");
    REGISTER_MODULE_CONSTANT(polygonFace, PolygonFace, All);
    REGISTER_MODULE_CONSTANT(polygonFace, PolygonFace, CW);
    REGISTER_MODULE_CONSTANT(polygonFace, PolygonFace, CCW);
    polygonFace.endModule();

    // DepthPassFunction
    auto depthPassFunction = module.beginModule("DepthPassFunction");
    REGISTER_MODULE_CONSTANT(depthPassFunction, DepthPassFunction, Never);
    REGISTER_MODULE_CONSTANT(depthPassFunction, DepthPassFunction, Always);
    REGISTER_MODULE_CONSTANT(depthPassFunction, DepthPassFunction, Equal);
    REGISTER_MODULE_CONSTANT(depthPassFunction, DepthPassFunction, NotEqual);
    REGISTER_MODULE_CONSTANT(depthPassFunction, DepthPassFunction, Less);
    REGISTER_MODULE_CONSTANT(depthPassFunction, DepthPassFunction, LEqual);
    REGISTER_MODULE_CONSTANT(depthPassFunction, DepthPassFunction, Greater);
    REGISTER_MODULE_CONSTANT(depthPassFunction, DepthPassFunction, GEqual);
    depthPassFunction.endModule();

    // Material
    auto mat = module.beginClass<Material>("Material");
    REGISTER_METHOD(mat, Material, getParameter);
    REGISTER_METHOD(mat, Material, getEffect);
    REGISTER_METHOD(mat, Material, getPolygonFace);
    REGISTER_METHOD(mat, Material, setPolygonFace);
    REGISTER_METHOD(mat, Material, isDepthWriteEnabled);
    REGISTER_METHOD(mat, Material, setDepthWriteEnabled);
    REGISTER_METHOD(mat, Material, isDepthTestEnabled);
    REGISTER_METHOD(mat, Material, setDepthTestEnabled);
    REGISTER_METHOD(mat, Material, getDepthPassFunction);
    REGISTER_METHOD(mat, Material, setDepthPassFunction);
    mat.endClass();

    // AutoBinding
    auto autoBinding = module.beginModule("AutoBinding");
    REGISTER_MODULE_CONSTANT(autoBinding, AutoBinding, None);
    REGISTER_MODULE_CONSTANT(autoBinding, AutoBinding, CameraWorldPosition);
    REGISTER_MODULE_CONSTANT(autoBinding, AutoBinding, InverseTransposedWorldMatrix);
    REGISTER_MODULE_CONSTANT(autoBinding, AutoBinding, InverseTransposedWorldViewMatrix);
    REGISTER_MODULE_CONSTANT(autoBinding, AutoBinding, ProjectionMatrix);
    REGISTER_MODULE_CONSTANT(autoBinding, AutoBinding, ViewMatrix);
    REGISTER_MODULE_CONSTANT(autoBinding, AutoBinding, ViewProjectionMatrix);
    REGISTER_MODULE_CONSTANT(autoBinding, AutoBinding, WorldMatrix);
    REGISTER_MODULE_CONSTANT(autoBinding, AutoBinding, WorldViewMatrix);
    REGISTER_MODULE_CONSTANT(autoBinding, AutoBinding, WorldViewProjectionMatrix);
    autoBinding.endModule();

    // MaterialParameter
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
    REGISTER_METHOD(mp, MaterialParameter, setFunction);
    REGISTER_METHOD(mp, MaterialParameter, bindValue);
    mp.endClass();

    // RenderContext
    auto rc = module.beginClass<RenderContext>("RenderContext");
    REGISTER_VARIABLE(rc, RenderContext, camera);
    REGISTER_VARIABLE(rc, RenderContext, cameraTransform);
    REGISTER_VARIABLE(rc, RenderContext, nodeTransform);
    REGISTER_VARIABLE(rc, RenderContext, scene);
    rc.endClass();

    // Scene
    auto scene = module.beginClass<Scene>("Scene");
    REGISTER_METHOD(scene, Scene, getDevice);
    REGISTER_METHOD(scene, Scene, createNode);
    scene.endClass();

    // KeyCode
    auto keyCode = module.beginModule("KeyCode");
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, A);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, B);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, C);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, D);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, E);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, F);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, G);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, H);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, I);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, J);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, K);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, L);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, M);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, N);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, O);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, P);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, Q);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, R);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, S);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, T);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, U);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, V);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, W);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, X);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, Y);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, Z);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, LeftArrow);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, RightArrow);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, UpArrow);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, DownArrow);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, Escape);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, Backspace);
    REGISTER_MODULE_CONSTANT(keyCode, KeyCode, Space);
    keyCode.endModule();

    // MouseButton
    auto mouseButton = module.beginModule("MouseButton");
    REGISTER_MODULE_CONSTANT(mouseButton, MouseButton, Left);
    REGISTER_MODULE_CONSTANT(mouseButton, MouseButton, Middle);
    REGISTER_MODULE_CONSTANT(mouseButton, MouseButton, Right);
    mouseButton.endModule();

    // DeviceMode
    auto deviceMode = module.beginModule("DeviceMode");
    REGISTER_MODULE_CONSTANT(deviceMode, DeviceMode, OpenGL);
    REGISTER_MODULE_CONSTANT(deviceMode, DeviceMode, Stub);
    mouseButton.endModule();

    // DeviceCreationArgs
    auto deviceCreationArgs = module.beginClass<DeviceCreationArgs>("DeviceCreationArgs");
    deviceCreationArgs.addConstructor(LUA_ARGS(_opt<DeviceMode>, _opt<int>, _opt<int>, _opt<bool>, _opt<std::string>, _opt<int>, _opt<int>, _opt<std::string>));
    REGISTER_VARIABLE(deviceCreationArgs, DeviceCreationArgs, mode);
    REGISTER_VARIABLE(deviceCreationArgs, DeviceCreationArgs, bits);
    REGISTER_VARIABLE(deviceCreationArgs, DeviceCreationArgs, canvasWidth);
    REGISTER_VARIABLE(deviceCreationArgs, DeviceCreationArgs, canvasHeight);
    REGISTER_VARIABLE(deviceCreationArgs, DeviceCreationArgs, depth);
    REGISTER_VARIABLE(deviceCreationArgs, DeviceCreationArgs, fullScreen);
    REGISTER_VARIABLE(deviceCreationArgs, DeviceCreationArgs, windowTitle);
    REGISTER_VARIABLE(deviceCreationArgs, DeviceCreationArgs, logFilePath);
    deviceCreationArgs.endClass();

    // Device
    auto device = module.beginClass<Device>("Device");
    REGISTER_STATIC_METHOD(device, Device, create);
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
    REGISTER_METHOD(device, Device, getScene);
    REGISTER_METHOD(device, Device, getResourceManager);
    REGISTER_METHOD(device, Device, getFileSystem);
    REGISTER_METHOD(device, Device, getLogger);
    REGISTER_METHOD(device, Device, getMode);
    REGISTER_METHOD(device, Device, run);
    REGISTER_METHOD(device, Device, setStartCallback);
    REGISTER_METHOD(device, Device, setShutdownCallback);
    REGISTER_METHOD(device, Device, setShutdownRequestedCallback);
    REGISTER_METHOD(device, Device, requestShutdown);
    REGISTER_METHOD(device, Device, shutdownRequested);
    device.endClass();

    // ResourceManager
    auto mgr = module.beginClass<ResourceManager>("ResourceManager");
    REGISTER_METHOD(mgr, ResourceManager, findEffect);
    REGISTER_METHOD(mgr, ResourceManager, findTexture2D);
    REGISTER_METHOD(mgr, ResourceManager, findCubeTexture);
    REGISTER_METHOD(mgr, ResourceManager, findMaterial);
    REGISTER_METHOD(mgr, ResourceManager, findMesh);
    REGISTER_METHOD(mgr, ResourceManager, findRenderTarget);
    REGISTER_METHOD(mgr, ResourceManager, findSurfaceRenderer);
    REGISTER_METHOD2(mgr, ResourceManager, getOrCreateEffect, LUA_ARGS(const std::string&, const std::string&, _opt<const std::string&>));
    REGISTER_METHOD2(mgr, ResourceManager, getOrCreatePrefabEffect, LUA_ARGS(EffectPrefab, _opt<const std::string&>));
    REGISTER_METHOD2(mgr, ResourceManager, getOrCreateTexture2D, LUA_ARGS(_opt<const std::string&>));
    REGISTER_METHOD2(mgr, ResourceManager, getOrCreateCubeTexture, LUA_ARGS(_opt<const std::string&>));
    REGISTER_METHOD2(mgr, ResourceManager, getOrCreateMaterial, LUA_ARGS(shared<Effect>, _opt<const std::string&>));
    REGISTER_METHOD2(mgr, ResourceManager, getOrCreateMesh, LUA_ARGS(const VertexFormat&, _opt<const std::string&>));
    REGISTER_METHOD2(mgr, ResourceManager, getOrCreatePrefabMesh, LUA_ARGS(MeshPrefab, _opt<const std::string&>));
    REGISTER_METHOD2(mgr, ResourceManager, getOrCreateRenderTarget, LUA_ARGS(_opt<const std::string&>));
    REGISTER_METHOD2(mgr, ResourceManager, getOrCreateSurfaceRenderer, LUA_ARGS(shared<Material>, _opt<const std::string&>));
    REGISTER_METHOD2(mgr, ResourceManager, getOrLoadTexture2D, LUA_ARGS(const std::string&, _opt<const std::string&>));
    REGISTER_METHOD2(mgr, ResourceManager, getOrLoadCubeTexture, LUA_ARGS(const std::vector<std::string>&, _opt<const std::string&>));
    REGISTER_METHOD2(mgr, ResourceManager, getOrLoadMesh, LUA_ARGS(const std::string&, _opt<const std::string&>));
    REGISTER_OVERLOADED_METHOD(mgr, ResourceManager, cleanUnusedResources, "cleanUnusedResources", void, , void);
    mgr.endClass();

    // SurfaceRenderer
    auto surfaceRenderer = module.beginClass<SurfaceRenderer>("SurfaceRenderer");
    REGISTER_METHOD(surfaceRenderer, LuaSurfaceRenderer, renderSurface);
    surfaceRenderer.endClass();

    // FileSystem
    auto fs = module.beginClass<FileSystem>("FileSystem");
    REGISTER_METHOD(fs, FileSystem, iterateLines);
    REGISTER_METHOD(fs, FileSystem, readLines);
    REGISTER_METHOD(fs, FileSystem, readText);
    REGISTER_METHOD(fs, FileSystem, writeLines);
    fs.endClass();

    // Logger
    auto logger = module.beginClass<Logger>("Logger");
    REGISTER_METHOD(logger, Logger, setTargetFile);
    REGISTER_METHOD(logger, Logger, logDebug);
    REGISTER_METHOD(logger, Logger, logInfo);
    REGISTER_METHOD(logger, Logger, logWarning);
    REGISTER_METHOD(logger, Logger, logError);
    REGISTER_METHOD(logger, Logger, logCritical);
    logger.endClass();

    module.endModule();
}