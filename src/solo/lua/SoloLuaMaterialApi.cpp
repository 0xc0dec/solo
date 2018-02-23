/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloMaterial.h"
#include "SoloLuaCommon.h"

using namespace solo;

void registerMaterialApi(CppBindModule<LuaBinding> &module)
{
    auto mat = BEGIN_CLASS(module, Material);
    REG_STATIC_METHOD(mat, Material, create);
    REG_METHOD(mat, Material, setFloatParameter);
    REG_METHOD(mat, Material, setVector2Parameter);
    REG_METHOD(mat, Material, setVector3Parameter);
    REG_METHOD(mat, Material, setVector4Parameter);
    REG_METHOD(mat, Material, setMatrixParameter);
    REG_METHOD(mat, Material, setTextureParameter);
    REG_METHOD(mat, Material, bindParameter);
    REG_METHOD(mat, Material, getEffect);
    REG_METHOD(mat, Material, getPolygonMode);
    REG_METHOD(mat, Material, setPolygonMode);
    REG_METHOD(mat, Material, getBlend);
    REG_METHOD(mat, Material, setBlend);
    REG_METHOD(mat, Material, getSrcBlendFactor);
    REG_METHOD(mat, Material, getDstBlendFactor);
    REG_METHOD(mat, Material, setBlendFactors);
    REG_METHOD(mat, Material, getFaceCull);
    REG_METHOD(mat, Material, setFaceCull);
    REG_METHOD(mat, Material, hasDepthWrite);
    REG_METHOD(mat, Material, setDepthWrite);
    REG_METHOD(mat, Material, hasDepthTest);
    REG_METHOD(mat, Material, setDepthTest);
    REG_METHOD(mat, Material, getDepthFunction);
    REG_METHOD(mat, Material, setDepthFunction);
    mat.endClass();
}
