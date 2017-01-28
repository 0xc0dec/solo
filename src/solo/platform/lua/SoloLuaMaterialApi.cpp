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

#include "SoloMaterial.h"
#include "SoloLuaCommon.h"

using namespace solo;


void registerMaterial(CppBindModule<LuaBinding> &module)
{
    auto mat = module.beginClass<Material>("Material");
    
    REG_STATIC_METHOD(mat, Material, create);
    
    REG_METHOD(mat, Material, setFloatParameter);
    REG_METHOD(mat, Material, setFloatArrayParameter);

    REG_METHOD(mat, Material, setVector2Parameter);
    REG_METHOD(mat, Material, setVector2ArrayParameter);

    REG_METHOD(mat, Material, setVector3Parameter);
    REG_METHOD(mat, Material, setVector3ArrayParameter);

    REG_METHOD(mat, Material, setVector4Parameter);
    REG_METHOD(mat, Material, setVector4ArrayParameter);

    REG_METHOD(mat, Material, setMatrixParameter);
    REG_METHOD(mat, Material, setMatrixArrayParameter);

    REG_METHOD(mat, Material, setTextureParameter);

    REG_METHOD(mat, Material, bindWorldMatrixParameter);
    REG_METHOD(mat, Material, bindViewMatrixParameter);
    REG_METHOD(mat, Material, bindProjectionMatrixParameter);
    REG_METHOD(mat, Material, bindWorldViewMatrixParameter);
    REG_METHOD(mat, Material, bindViewProjectionMatrixParameter);
    REG_METHOD(mat, Material, bindWorldViewProjectionMatrixParameter);
    REG_METHOD(mat, Material, bindInvTransposedWorldMatrixParameter);
    REG_METHOD(mat, Material, bindInvTransposedWorldViewMatrixParameter);
    REG_METHOD(mat, Material, bindCameraWorldPositionParameter);
    
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
    
    REG_METHOD(mat, Material, getDepthWrite);
    REG_METHOD(mat, Material, setDepthWrite);

    REG_METHOD(mat, Material, getDepthTest);
    REG_METHOD(mat, Material, setDepthTest);

    REG_METHOD(mat, Material, getDepthFunction);
    REG_METHOD(mat, Material, setDepthFunction);

    mat.endClass();
}