/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloTransform.h"
#include "SoloTexture.h"
#include "SoloLuaCommon.h"
#include "SoloEffect.h"
#include "SoloDevice.h"
#include "SoloMaterial.h"
#include "SoloMesh.h"

using namespace solo;

void registerEnums(CppBindModule<LuaBinding> &module)
{
    auto ts = module.beginModule("TransformSpace");
    REG_MODULE_CONSTANT(ts, TransformSpace, Parent);
    REG_MODULE_CONSTANT(ts, TransformSpace, Self);
    REG_MODULE_CONSTANT(ts, TransformSpace, World);
    ts.endModule();

    auto w = module.beginModule("TextureWrapping");
    REG_MODULE_CONSTANT(w, TextureWrapping, Clamp);
    REG_MODULE_CONSTANT(w, TextureWrapping, Repeat);
    w.endModule();

    auto f = module.beginModule("TextureFormat");
    REG_MODULE_CONSTANT(f, TextureFormat, RGB);
    REG_MODULE_CONSTANT(f, TextureFormat, RGBA);
    REG_MODULE_CONSTANT(f, TextureFormat, Red);
    w.endModule();

    auto tf = module.beginModule("TextureFiltering");
    REG_MODULE_CONSTANT(tf, TextureFiltering, Nearest);
    REG_MODULE_CONSTANT(tf, TextureFiltering, Linear);
    REG_MODULE_CONSTANT(tf, TextureFiltering, LinearMipmapLinear);
    REG_MODULE_CONSTANT(tf, TextureFiltering, LinearMipmapNearest);
    REG_MODULE_CONSTANT(tf, TextureFiltering, NearestMipmapLinear);
    REG_MODULE_CONSTANT(tf, TextureFiltering, NearestMipmapNearest);
    tf.endModule();

    auto ep = module.beginModule("EffectPrefab");
    REG_MODULE_CONSTANT(ep, EffectPrefab, Skybox);
    REG_MODULE_CONSTANT(ep, EffectPrefab, Font);
    ep.endModule();

    auto fc = module.beginModule("FaceCull");
    REG_MODULE_CONSTANT(fc, FaceCull, None);
    REG_MODULE_CONSTANT(fc, FaceCull, Back);
    REG_MODULE_CONSTANT(fc, FaceCull, Front);
    fc.endModule();

    auto pm = module.beginModule("PolygonMode");
    REG_MODULE_CONSTANT(pm, PolygonMode, Points);
    REG_MODULE_CONSTANT(pm, PolygonMode, Fill);
    REG_MODULE_CONSTANT(pm, PolygonMode, Wireframe);
    pm.endModule();

    auto df = module.beginModule("DepthFunction");
    REG_MODULE_CONSTANT(df, DepthFunction, Never);
    REG_MODULE_CONSTANT(df, DepthFunction, Always);
    REG_MODULE_CONSTANT(df, DepthFunction, Equal);
    REG_MODULE_CONSTANT(df, DepthFunction, NotEqual);
    REG_MODULE_CONSTANT(df, DepthFunction, Less);
    REG_MODULE_CONSTANT(df, DepthFunction, LEqual);
    REG_MODULE_CONSTANT(df, DepthFunction, Greater);
    REG_MODULE_CONSTANT(df, DepthFunction, GEqual);
    df.endModule();

    auto bf = module.beginModule("BlendFactor");
    REG_MODULE_CONSTANT(bf, BlendFactor, Zero);
    REG_MODULE_CONSTANT(bf, BlendFactor, One);
    REG_MODULE_CONSTANT(bf, BlendFactor, SrcColor);
    REG_MODULE_CONSTANT(bf, BlendFactor, OneMinusSrcColor);
    REG_MODULE_CONSTANT(bf, BlendFactor, DstColor);
    REG_MODULE_CONSTANT(bf, BlendFactor, OneMinusDstColor);
    REG_MODULE_CONSTANT(bf, BlendFactor, SrcAlpha);
    REG_MODULE_CONSTANT(bf, BlendFactor, OneMinusSrcAlpha);
    REG_MODULE_CONSTANT(bf, BlendFactor, DstAlpha);
    REG_MODULE_CONSTANT(bf, BlendFactor, OneMinusDstAlpha);
    REG_MODULE_CONSTANT(bf, BlendFactor, ConstantAlpha);
    REG_MODULE_CONSTANT(bf, BlendFactor, OneMinusConstantAlpha);
    REG_MODULE_CONSTANT(bf, BlendFactor, SrcAlphaSaturate);
    bf.endModule();

    auto bps = module.beginModule("BindParameterSemantics");
    REG_MODULE_CONSTANT(bps, BindParameterSemantics, WorldMatrix);
    REG_MODULE_CONSTANT(bps, BindParameterSemantics, ViewMatrix);
    REG_MODULE_CONSTANT(bps, BindParameterSemantics, ProjectionMatrix);
    REG_MODULE_CONSTANT(bps, BindParameterSemantics, WorldViewMatrix);
    REG_MODULE_CONSTANT(bps, BindParameterSemantics, ViewProjectionMatrix);
    REG_MODULE_CONSTANT(bps, BindParameterSemantics, WorldViewProjectionMatrix);
    REG_MODULE_CONSTANT(bps, BindParameterSemantics, InverseTransposedWorldMatrix);
    REG_MODULE_CONSTANT(bps, BindParameterSemantics, InverseTransposedWorldViewMatrix);
    REG_MODULE_CONSTANT(bps, BindParameterSemantics, CameraWorldPosition);

    auto kc = module.beginModule("KeyCode");
    REG_MODULE_CONSTANT(kc, KeyCode, Digit0);
    REG_MODULE_CONSTANT(kc, KeyCode, Digit1);
    REG_MODULE_CONSTANT(kc, KeyCode, Digit2);
    REG_MODULE_CONSTANT(kc, KeyCode, Digit3);
    REG_MODULE_CONSTANT(kc, KeyCode, Digit4);
    REG_MODULE_CONSTANT(kc, KeyCode, Digit5);
    REG_MODULE_CONSTANT(kc, KeyCode, Digit6);
    REG_MODULE_CONSTANT(kc, KeyCode, Digit7);
    REG_MODULE_CONSTANT(kc, KeyCode, Digit8);
    REG_MODULE_CONSTANT(kc, KeyCode, Digit9);
    REG_MODULE_CONSTANT(kc, KeyCode, A);
    REG_MODULE_CONSTANT(kc, KeyCode, B);
    REG_MODULE_CONSTANT(kc, KeyCode, C);
    REG_MODULE_CONSTANT(kc, KeyCode, D);
    REG_MODULE_CONSTANT(kc, KeyCode, E);
    REG_MODULE_CONSTANT(kc, KeyCode, F);
    REG_MODULE_CONSTANT(kc, KeyCode, G);
    REG_MODULE_CONSTANT(kc, KeyCode, H);
    REG_MODULE_CONSTANT(kc, KeyCode, I);
    REG_MODULE_CONSTANT(kc, KeyCode, J);
    REG_MODULE_CONSTANT(kc, KeyCode, K);
    REG_MODULE_CONSTANT(kc, KeyCode, L);
    REG_MODULE_CONSTANT(kc, KeyCode, M);
    REG_MODULE_CONSTANT(kc, KeyCode, N);
    REG_MODULE_CONSTANT(kc, KeyCode, O);
    REG_MODULE_CONSTANT(kc, KeyCode, P);
    REG_MODULE_CONSTANT(kc, KeyCode, Q);
    REG_MODULE_CONSTANT(kc, KeyCode, R);
    REG_MODULE_CONSTANT(kc, KeyCode, S);
    REG_MODULE_CONSTANT(kc, KeyCode, T);
    REG_MODULE_CONSTANT(kc, KeyCode, U);
    REG_MODULE_CONSTANT(kc, KeyCode, V);
    REG_MODULE_CONSTANT(kc, KeyCode, W);
    REG_MODULE_CONSTANT(kc, KeyCode, X);
    REG_MODULE_CONSTANT(kc, KeyCode, Y);
    REG_MODULE_CONSTANT(kc, KeyCode, Z);
    REG_MODULE_CONSTANT(kc, KeyCode, LeftArrow);
    REG_MODULE_CONSTANT(kc, KeyCode, RightArrow);
    REG_MODULE_CONSTANT(kc, KeyCode, UpArrow);
    REG_MODULE_CONSTANT(kc, KeyCode, DownArrow);
    REG_MODULE_CONSTANT(kc, KeyCode, Escape);
    REG_MODULE_CONSTANT(kc, KeyCode, Backspace);
    REG_MODULE_CONSTANT(kc, KeyCode, Space);
    REG_MODULE_CONSTANT(kc, KeyCode, Return);
    REG_MODULE_CONSTANT(kc, KeyCode, LeftShift);
    REG_MODULE_CONSTANT(kc, KeyCode, LeftCtrl);
    REG_MODULE_CONSTANT(kc, KeyCode, LeftAlt);
    REG_MODULE_CONSTANT(kc, KeyCode, RightShift);
    REG_MODULE_CONSTANT(kc, KeyCode, RightCtrl);
    REG_MODULE_CONSTANT(kc, KeyCode, RightAlt);
    kc.endModule();

    auto mb = module.beginModule("MouseButton");
    REG_MODULE_CONSTANT(mb, MouseButton, Left);
    REG_MODULE_CONSTANT(mb, MouseButton, Middle);
    REG_MODULE_CONSTANT(mb, MouseButton, Right);
    mb.endModule();

    auto dm = module.beginModule("DeviceMode");
    REG_MODULE_CONSTANT(dm, DeviceMode, Null);
    REG_MODULE_CONSTANT(dm, DeviceMode, OpenGL);
    REG_MODULE_CONSTANT(dm, DeviceMode, Vulkan);
    dm.endModule();

    auto mp = module.beginModule("MeshPrefab");
    REG_MODULE_CONSTANT(mp, MeshPrefab, Cube);
    REG_MODULE_CONSTANT(mp, MeshPrefab, Quad);
    mp.endModule();

    auto pt = module.beginModule("PrimitiveType");
    REG_MODULE_CONSTANT(pt, PrimitiveType, Triangles);
    REG_MODULE_CONSTANT(pt, PrimitiveType, TriangleStrip);
    REG_MODULE_CONSTANT(pt, PrimitiveType, Lines);
    REG_MODULE_CONSTANT(pt, PrimitiveType, LineStrip);
    REG_MODULE_CONSTANT(pt, PrimitiveType, Points);
    pt.endModule();
}
