/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloMaterial.h"
#include "SoloLuaCommon.h"

using namespace solo;

void registerMaterialApi(CppBindModule<LuaBinding> &module) {
    auto binding = BEGIN_CLASS(module, Material);
    REG_STATIC_METHOD(binding, Material, fromEffect);
    REG_METHOD(binding, Material, setFloatParameter);
    REG_METHOD(binding, Material, setVector2Parameter);
    REG_METHOD(binding, Material, setVector3Parameter);
    REG_METHOD(binding, Material, setVector4Parameter);
    REG_METHOD(binding, Material, setMatrixParameter);
    REG_METHOD(binding, Material, setTextureParameter);
    REG_METHOD(binding, Material, bindFloatParameter);
    REG_METHOD(binding, Material, bindVector2Parameter);
    REG_METHOD(binding, Material, bindVector3Parameter);
    REG_METHOD(binding, Material, bindVector4Parameter);
    REG_METHOD(binding, Material, bindMatrixParameter);
    REG_METHOD(binding, Material, bindParameter);
    REG_METHOD(binding, Material, effect);
    REG_METHOD(binding, Material, clone);
    REG_METHOD(binding, Material, polygonMode);
    REG_METHOD(binding, Material, setPolygonMode);
    REG_METHOD(binding, Material, hasBlend);
    REG_METHOD(binding, Material, setBlend);
    REG_METHOD(binding, Material, srcBlendFactor);
    REG_METHOD(binding, Material, dstBlendFactor);
    REG_METHOD(binding, Material, setBlendFactors);
    REG_METHOD(binding, Material, faceCull);
    REG_METHOD(binding, Material, setFaceCull);
    REG_METHOD(binding, Material, hasDepthWrite);
    REG_METHOD(binding, Material, setDepthWrite);
    REG_METHOD(binding, Material, hasDepthTest);
    REG_METHOD(binding, Material, setDepthTest);
    REG_METHOD(binding, Material, depthFunction);
    REG_METHOD(binding, Material, setDepthFunction);
    REG_PTR_EQUALITY(binding, Material);
    binding.endClass();
}
