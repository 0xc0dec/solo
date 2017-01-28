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

#include "SoloTransform.h"
#include "SoloPlane.h"
#include "SoloTexture.h"
#include "SoloLuaCommon.h"
#include "SoloEffect.h"
#include "SoloDevice.h"
#include "SoloMaterialParameter.h"

using namespace solo;


void registerEnums(CppBindModule<LuaBinding> &module)
{
    auto transformSpace = module.beginModule("TransformSpace");
    REG_MODULE_CONSTANT(transformSpace, TransformSpace, Parent);
    REG_MODULE_CONSTANT(transformSpace, TransformSpace, Self);
    REG_MODULE_CONSTANT(transformSpace, TransformSpace, World);
    transformSpace.endModule();

    auto intersection = module.beginModule("PlaneIntersection");
    REG_MODULE_CONSTANT(intersection, PlaneIntersection, Intersecting);
    REG_MODULE_CONSTANT(intersection, PlaneIntersection, Front);
    REG_MODULE_CONSTANT(intersection, PlaneIntersection, Back);
    intersection.endModule();

    auto textureWrapping = module.beginModule("TextureWrapping");
    REG_MODULE_CONSTANT(textureWrapping, TextureWrapping, Clamp);
    REG_MODULE_CONSTANT(textureWrapping, TextureWrapping, Repeat);
    textureWrapping.endModule();

    auto textureFormat = module.beginModule("TextureFormat");
    REG_MODULE_CONSTANT(textureFormat, TextureFormat, Alpha);
    REG_MODULE_CONSTANT(textureFormat, TextureFormat, RGB);
    REG_MODULE_CONSTANT(textureFormat, TextureFormat, RGBA);
    REG_MODULE_CONSTANT(textureFormat, TextureFormat, Red);
    textureWrapping.endModule();

    auto textureFiltering = module.beginModule("TextureFiltering");
    REG_MODULE_CONSTANT(textureFiltering, TextureFiltering, Nearest);
    REG_MODULE_CONSTANT(textureFiltering, TextureFiltering, Linear);
    REG_MODULE_CONSTANT(textureFiltering, TextureFiltering, LinearMipmapLinear);
    REG_MODULE_CONSTANT(textureFiltering, TextureFiltering, LinearMipmapNearest);
    REG_MODULE_CONSTANT(textureFiltering, TextureFiltering, NearestMipmapLinear);
    REG_MODULE_CONSTANT(textureFiltering, TextureFiltering, NearestMipmapNearest);
    textureFiltering.endModule();

    auto cubeTextureFace = module.beginModule("CubeTextureFace");
    REG_MODULE_CONSTANT(cubeTextureFace, CubeTextureFace, Front);
    REG_MODULE_CONSTANT(cubeTextureFace, CubeTextureFace, Back);
    REG_MODULE_CONSTANT(cubeTextureFace, CubeTextureFace, Left);
    REG_MODULE_CONSTANT(cubeTextureFace, CubeTextureFace, Right);
    REG_MODULE_CONSTANT(cubeTextureFace, CubeTextureFace, Top);
    REG_MODULE_CONSTANT(cubeTextureFace, CubeTextureFace, Bottom);
    cubeTextureFace.endModule();

    auto effectPrefab = module.beginModule("EffectPrefab");
    REG_MODULE_CONSTANT(effectPrefab, EffectPrefab, Skybox);
    REG_MODULE_CONSTANT(effectPrefab, EffectPrefab, Font);
    effectPrefab.endModule();

    auto faceCull = module.beginModule("FaceCull");
    REG_MODULE_CONSTANT(faceCull, FaceCull, All);
    REG_MODULE_CONSTANT(faceCull, FaceCull, CW);
    REG_MODULE_CONSTANT(faceCull, FaceCull, CCW);
    faceCull.endModule();

    auto polygonMode = module.beginModule("PolygonMode");
    REG_MODULE_CONSTANT(polygonMode, PolygonMode, Points);
    REG_MODULE_CONSTANT(polygonMode, PolygonMode, Triangle);
    REG_MODULE_CONSTANT(polygonMode, PolygonMode, Wireframe);
    polygonMode.endModule();

    auto depthFunction = module.beginModule("DepthFunction");
    REG_MODULE_CONSTANT(depthFunction, DepthFunction, Never);
    REG_MODULE_CONSTANT(depthFunction, DepthFunction, Always);
    REG_MODULE_CONSTANT(depthFunction, DepthFunction, Equal);
    REG_MODULE_CONSTANT(depthFunction, DepthFunction, NotEqual);
    REG_MODULE_CONSTANT(depthFunction, DepthFunction, Less);
    REG_MODULE_CONSTANT(depthFunction, DepthFunction, LEqual);
    REG_MODULE_CONSTANT(depthFunction, DepthFunction, Greater);
    REG_MODULE_CONSTANT(depthFunction, DepthFunction, GEqual);
    depthFunction.endModule();

    auto blendFactor = module.beginModule("BlendFactor");
    REG_MODULE_CONSTANT(blendFactor, BlendFactor, Zero);
    REG_MODULE_CONSTANT(blendFactor, BlendFactor, One);
    REG_MODULE_CONSTANT(blendFactor, BlendFactor, SrcColor);
    REG_MODULE_CONSTANT(blendFactor, BlendFactor, OneMinusSrcColor);
    REG_MODULE_CONSTANT(blendFactor, BlendFactor, DstColor);
    REG_MODULE_CONSTANT(blendFactor, BlendFactor, OneMinusDstColor);
    REG_MODULE_CONSTANT(blendFactor, BlendFactor, SrcAlpha);
    REG_MODULE_CONSTANT(blendFactor, BlendFactor, OneMinusSrcAlpha);
    REG_MODULE_CONSTANT(blendFactor, BlendFactor, DstAlpha);
    REG_MODULE_CONSTANT(blendFactor, BlendFactor, OneMinusDstAlpha);
    REG_MODULE_CONSTANT(blendFactor, BlendFactor, ConstantAlpha);
    REG_MODULE_CONSTANT(blendFactor, BlendFactor, OneMinusConstantAlpha);
    REG_MODULE_CONSTANT(blendFactor, BlendFactor, SrcAlphaSaturate);
    blendFactor.endModule();

    auto autoBinding = module.beginModule("MaterialParameterType");
    REG_MODULE_CONSTANT(autoBinding, MaterialParameterType, Float);
    REG_MODULE_CONSTANT(autoBinding, MaterialParameterType, FloatArray);
    REG_MODULE_CONSTANT(autoBinding, MaterialParameterType, Vector2);
    REG_MODULE_CONSTANT(autoBinding, MaterialParameterType, Vector2Array);
    REG_MODULE_CONSTANT(autoBinding, MaterialParameterType, Vector3);
    REG_MODULE_CONSTANT(autoBinding, MaterialParameterType, Vector3Array);
    REG_MODULE_CONSTANT(autoBinding, MaterialParameterType, Vector4);
    REG_MODULE_CONSTANT(autoBinding, MaterialParameterType, Vector4Array);
    REG_MODULE_CONSTANT(autoBinding, MaterialParameterType, Matrix);
    REG_MODULE_CONSTANT(autoBinding, MaterialParameterType, MatrixArray);
    REG_MODULE_CONSTANT(autoBinding, MaterialParameterType, Texture);
    REG_MODULE_CONSTANT(autoBinding, MaterialParameterType, CameraWorldPosition);
    REG_MODULE_CONSTANT(autoBinding, MaterialParameterType, InverseTransposedWorldMatrix);
    REG_MODULE_CONSTANT(autoBinding, MaterialParameterType, InverseTransposedWorldViewMatrix);
    REG_MODULE_CONSTANT(autoBinding, MaterialParameterType, ProjectionMatrix);
    REG_MODULE_CONSTANT(autoBinding, MaterialParameterType, ViewMatrix);
    REG_MODULE_CONSTANT(autoBinding, MaterialParameterType, ViewProjectionMatrix);
    REG_MODULE_CONSTANT(autoBinding, MaterialParameterType, WorldMatrix);
    REG_MODULE_CONSTANT(autoBinding, MaterialParameterType, WorldViewMatrix);
    REG_MODULE_CONSTANT(autoBinding, MaterialParameterType, WorldViewProjectionMatrix);
    autoBinding.endModule();

    auto keyCode = module.beginModule("KeyCode");
    REG_MODULE_CONSTANT(keyCode, KeyCode, Digit0);
    REG_MODULE_CONSTANT(keyCode, KeyCode, Digit1);
    REG_MODULE_CONSTANT(keyCode, KeyCode, Digit2);
    REG_MODULE_CONSTANT(keyCode, KeyCode, Digit3);
    REG_MODULE_CONSTANT(keyCode, KeyCode, Digit4);
    REG_MODULE_CONSTANT(keyCode, KeyCode, Digit5);
    REG_MODULE_CONSTANT(keyCode, KeyCode, Digit6);
    REG_MODULE_CONSTANT(keyCode, KeyCode, Digit7);
    REG_MODULE_CONSTANT(keyCode, KeyCode, Digit8);
    REG_MODULE_CONSTANT(keyCode, KeyCode, Digit9);
    REG_MODULE_CONSTANT(keyCode, KeyCode, A);
    REG_MODULE_CONSTANT(keyCode, KeyCode, B);
    REG_MODULE_CONSTANT(keyCode, KeyCode, C);
    REG_MODULE_CONSTANT(keyCode, KeyCode, D);
    REG_MODULE_CONSTANT(keyCode, KeyCode, E);
    REG_MODULE_CONSTANT(keyCode, KeyCode, F);
    REG_MODULE_CONSTANT(keyCode, KeyCode, G);
    REG_MODULE_CONSTANT(keyCode, KeyCode, H);
    REG_MODULE_CONSTANT(keyCode, KeyCode, I);
    REG_MODULE_CONSTANT(keyCode, KeyCode, J);
    REG_MODULE_CONSTANT(keyCode, KeyCode, K);
    REG_MODULE_CONSTANT(keyCode, KeyCode, L);
    REG_MODULE_CONSTANT(keyCode, KeyCode, M);
    REG_MODULE_CONSTANT(keyCode, KeyCode, N);
    REG_MODULE_CONSTANT(keyCode, KeyCode, O);
    REG_MODULE_CONSTANT(keyCode, KeyCode, P);
    REG_MODULE_CONSTANT(keyCode, KeyCode, Q);
    REG_MODULE_CONSTANT(keyCode, KeyCode, R);
    REG_MODULE_CONSTANT(keyCode, KeyCode, S);
    REG_MODULE_CONSTANT(keyCode, KeyCode, T);
    REG_MODULE_CONSTANT(keyCode, KeyCode, U);
    REG_MODULE_CONSTANT(keyCode, KeyCode, V);
    REG_MODULE_CONSTANT(keyCode, KeyCode, W);
    REG_MODULE_CONSTANT(keyCode, KeyCode, X);
    REG_MODULE_CONSTANT(keyCode, KeyCode, Y);
    REG_MODULE_CONSTANT(keyCode, KeyCode, Z);
    REG_MODULE_CONSTANT(keyCode, KeyCode, LeftArrow);
    REG_MODULE_CONSTANT(keyCode, KeyCode, RightArrow);
    REG_MODULE_CONSTANT(keyCode, KeyCode, UpArrow);
    REG_MODULE_CONSTANT(keyCode, KeyCode, DownArrow);
    REG_MODULE_CONSTANT(keyCode, KeyCode, Escape);
    REG_MODULE_CONSTANT(keyCode, KeyCode, Backspace);
    REG_MODULE_CONSTANT(keyCode, KeyCode, Space);
    REG_MODULE_CONSTANT(keyCode, KeyCode, Return);
    REG_MODULE_CONSTANT(keyCode, KeyCode, LeftShift);
    REG_MODULE_CONSTANT(keyCode, KeyCode, LeftCtrl);
    REG_MODULE_CONSTANT(keyCode, KeyCode, LeftAlt);
    REG_MODULE_CONSTANT(keyCode, KeyCode, RightShift);
    REG_MODULE_CONSTANT(keyCode, KeyCode, RightCtrl);
    REG_MODULE_CONSTANT(keyCode, KeyCode, RightAlt);
    keyCode.endModule();

    auto mouseButton = module.beginModule("MouseButton");
    REG_MODULE_CONSTANT(mouseButton, MouseButton, Left);
    REG_MODULE_CONSTANT(mouseButton, MouseButton, Middle);
    REG_MODULE_CONSTANT(mouseButton, MouseButton, Right);
    mouseButton.endModule();
}
