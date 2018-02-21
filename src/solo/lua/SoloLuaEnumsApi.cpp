/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloTransform.h"
#include "SoloTexture.h"
#include "SoloLuaCommon.h"
#include "SoloDevice.h"
#include "SoloMaterial.h"
#include "SoloMesh.h"

using namespace solo;

void registerEnums(CppBindModule<LuaBinding> &module)
{
    {
        auto m = module.beginModule("TransformSpace");
        REG_MODULE_CONSTANT(m, TransformSpace, Parent);
        REG_MODULE_CONSTANT(m, TransformSpace, Self);
        REG_MODULE_CONSTANT(m, TransformSpace, World);
        m.endModule();
    }

    {
        auto m = module.beginModule("TextureWrap");
        REG_MODULE_CONSTANT(m, TextureWrap, ClampToEdge);
        REG_MODULE_CONSTANT(m, TextureWrap, ClampToBorder);
        REG_MODULE_CONSTANT(m, TextureWrap, Repeat);
        REG_MODULE_CONSTANT(m, TextureWrap, MirrorRepeat);
        m.endModule();
    }

    {
        auto m = module.beginModule("TextureFormat");
        REG_MODULE_CONSTANT(m, TextureFormat, RGB8);
        REG_MODULE_CONSTANT(m, TextureFormat, RGBA8);
        REG_MODULE_CONSTANT(m, TextureFormat, RGBA16F);
        REG_MODULE_CONSTANT(m, TextureFormat, R8);
        REG_MODULE_CONSTANT(m, TextureFormat, Depth24);
        m.endModule();
    }

    {
        auto m = module.beginModule("TextureFilter");
        REG_MODULE_CONSTANT(m, TextureFilter, Nearest);
        REG_MODULE_CONSTANT(m, TextureFilter, Linear);
        m.endModule();
    }

    {
        auto m = module.beginModule("TextureMipFilter");
        REG_MODULE_CONSTANT(m, TextureMipFilter, Nearest);
        REG_MODULE_CONSTANT(m, TextureMipFilter, Linear);
        REG_MODULE_CONSTANT(m, TextureMipFilter, None);
        m.endModule();
    }

    {
        auto m = module.beginModule("MaterialPrefab");
        REG_MODULE_CONSTANT(m, MaterialPrefab, Font);
        m.endModule();
    }

    {
        auto m = module.beginModule("FaceCull");
        REG_MODULE_CONSTANT(m, FaceCull, None);
        REG_MODULE_CONSTANT(m, FaceCull, Back);
        REG_MODULE_CONSTANT(m, FaceCull, Front);
        m.endModule();
    }

    {
        auto m = module.beginModule("PolygonMode");
        REG_MODULE_CONSTANT(m, PolygonMode, Points);
        REG_MODULE_CONSTANT(m, PolygonMode, Fill);
        REG_MODULE_CONSTANT(m, PolygonMode, Wireframe);
        m.endModule();
    }

    {
        auto m = module.beginModule("DepthFunction");
        REG_MODULE_CONSTANT(m, DepthFunction, Never);
        REG_MODULE_CONSTANT(m, DepthFunction, Always);
        REG_MODULE_CONSTANT(m, DepthFunction, Equal);
        REG_MODULE_CONSTANT(m, DepthFunction, NotEqual);
        REG_MODULE_CONSTANT(m, DepthFunction, Less);
        REG_MODULE_CONSTANT(m, DepthFunction, LEqual);
        REG_MODULE_CONSTANT(m, DepthFunction, Greater);
        REG_MODULE_CONSTANT(m, DepthFunction, GEqual);
        m.endModule();
    }

    {
        auto m = module.beginModule("BlendFactor");
        REG_MODULE_CONSTANT(m, BlendFactor, Zero);
        REG_MODULE_CONSTANT(m, BlendFactor, One);
        REG_MODULE_CONSTANT(m, BlendFactor, SrcColor);
        REG_MODULE_CONSTANT(m, BlendFactor, OneMinusSrcColor);
        REG_MODULE_CONSTANT(m, BlendFactor, DstColor);
        REG_MODULE_CONSTANT(m, BlendFactor, OneMinusDstColor);
        REG_MODULE_CONSTANT(m, BlendFactor, SrcAlpha);
        REG_MODULE_CONSTANT(m, BlendFactor, OneMinusSrcAlpha);
        REG_MODULE_CONSTANT(m, BlendFactor, DstAlpha);
        REG_MODULE_CONSTANT(m, BlendFactor, OneMinusDstAlpha);
        REG_MODULE_CONSTANT(m, BlendFactor, ConstantAlpha);
        REG_MODULE_CONSTANT(m, BlendFactor, OneMinusConstantAlpha);
        REG_MODULE_CONSTANT(m, BlendFactor, SrcAlphaSaturate);
        m.endModule();
    }

    {
        auto m = module.beginModule("BindParameterSemantics");
        REG_MODULE_CONSTANT(m, BindParameterSemantics, WorldMatrix);
        REG_MODULE_CONSTANT(m, BindParameterSemantics, ViewMatrix);
        REG_MODULE_CONSTANT(m, BindParameterSemantics, ProjectionMatrix);
        REG_MODULE_CONSTANT(m, BindParameterSemantics, WorldViewMatrix);
        REG_MODULE_CONSTANT(m, BindParameterSemantics, ViewProjectionMatrix);
        REG_MODULE_CONSTANT(m, BindParameterSemantics, WorldViewProjectionMatrix);
        REG_MODULE_CONSTANT(m, BindParameterSemantics, InverseTransposedWorldMatrix);
        REG_MODULE_CONSTANT(m, BindParameterSemantics, InverseTransposedWorldViewMatrix);
        REG_MODULE_CONSTANT(m, BindParameterSemantics, CameraWorldPosition);
        m.endModule();
    }

    {
        auto m = module.beginModule("KeyCode");
        REG_MODULE_CONSTANT(m, KeyCode, Digit0);
        REG_MODULE_CONSTANT(m, KeyCode, Digit1);
        REG_MODULE_CONSTANT(m, KeyCode, Digit2);
        REG_MODULE_CONSTANT(m, KeyCode, Digit3);
        REG_MODULE_CONSTANT(m, KeyCode, Digit4);
        REG_MODULE_CONSTANT(m, KeyCode, Digit5);
        REG_MODULE_CONSTANT(m, KeyCode, Digit6);
        REG_MODULE_CONSTANT(m, KeyCode, Digit7);
        REG_MODULE_CONSTANT(m, KeyCode, Digit8);
        REG_MODULE_CONSTANT(m, KeyCode, Digit9);
        REG_MODULE_CONSTANT(m, KeyCode, A);
        REG_MODULE_CONSTANT(m, KeyCode, B);
        REG_MODULE_CONSTANT(m, KeyCode, C);
        REG_MODULE_CONSTANT(m, KeyCode, D);
        REG_MODULE_CONSTANT(m, KeyCode, E);
        REG_MODULE_CONSTANT(m, KeyCode, F);
        REG_MODULE_CONSTANT(m, KeyCode, G);
        REG_MODULE_CONSTANT(m, KeyCode, H);
        REG_MODULE_CONSTANT(m, KeyCode, I);
        REG_MODULE_CONSTANT(m, KeyCode, J);
        REG_MODULE_CONSTANT(m, KeyCode, K);
        REG_MODULE_CONSTANT(m, KeyCode, L);
        REG_MODULE_CONSTANT(m, KeyCode, M);
        REG_MODULE_CONSTANT(m, KeyCode, N);
        REG_MODULE_CONSTANT(m, KeyCode, O);
        REG_MODULE_CONSTANT(m, KeyCode, P);
        REG_MODULE_CONSTANT(m, KeyCode, Q);
        REG_MODULE_CONSTANT(m, KeyCode, R);
        REG_MODULE_CONSTANT(m, KeyCode, S);
        REG_MODULE_CONSTANT(m, KeyCode, T);
        REG_MODULE_CONSTANT(m, KeyCode, U);
        REG_MODULE_CONSTANT(m, KeyCode, V);
        REG_MODULE_CONSTANT(m, KeyCode, W);
        REG_MODULE_CONSTANT(m, KeyCode, X);
        REG_MODULE_CONSTANT(m, KeyCode, Y);
        REG_MODULE_CONSTANT(m, KeyCode, Z);
        REG_MODULE_CONSTANT(m, KeyCode, LeftArrow);
        REG_MODULE_CONSTANT(m, KeyCode, RightArrow);
        REG_MODULE_CONSTANT(m, KeyCode, UpArrow);
        REG_MODULE_CONSTANT(m, KeyCode, DownArrow);
        REG_MODULE_CONSTANT(m, KeyCode, Escape);
        REG_MODULE_CONSTANT(m, KeyCode, Backspace);
        REG_MODULE_CONSTANT(m, KeyCode, Space);
        REG_MODULE_CONSTANT(m, KeyCode, Return);
        REG_MODULE_CONSTANT(m, KeyCode, LeftShift);
        REG_MODULE_CONSTANT(m, KeyCode, LeftCtrl);
        REG_MODULE_CONSTANT(m, KeyCode, LeftAlt);
        REG_MODULE_CONSTANT(m, KeyCode, RightShift);
        REG_MODULE_CONSTANT(m, KeyCode, RightCtrl);
        REG_MODULE_CONSTANT(m, KeyCode, RightAlt);
        m.endModule();
    }

    {
        auto m = module.beginModule("MouseButton");
        REG_MODULE_CONSTANT(m, MouseButton, Left);
        REG_MODULE_CONSTANT(m, MouseButton, Middle);
        REG_MODULE_CONSTANT(m, MouseButton, Right);
        m.endModule();
    }
    
    {
        auto m = module.beginModule("DeviceMode");
        REG_MODULE_CONSTANT(m, DeviceMode, Null);
        REG_MODULE_CONSTANT(m, DeviceMode, OpenGL);
        REG_MODULE_CONSTANT(m, DeviceMode, Vulkan);
        m.endModule();
    }

    {
        auto m = module.beginModule("MeshPrefab");
        REG_MODULE_CONSTANT(m, MeshPrefab, Cube);
        REG_MODULE_CONSTANT(m, MeshPrefab, Quad);
        m.endModule();
    }

    {
        auto m = module.beginModule("PrimitiveType");
        REG_MODULE_CONSTANT(m, PrimitiveType, Triangles);
        REG_MODULE_CONSTANT(m, PrimitiveType, TriangleStrip);
        REG_MODULE_CONSTANT(m, PrimitiveType, Lines);
        REG_MODULE_CONSTANT(m, PrimitiveType, LineStrip);
        REG_MODULE_CONSTANT(m, PrimitiveType, Points);
        m.endModule();
    }

    {
        auto m = module.beginModule("VertexAttributeSemantics");
        REG_MODULE_CONSTANT(m, VertexAttributeSemantics, Position);
        REG_MODULE_CONSTANT(m, VertexAttributeSemantics, Normal);
        REG_MODULE_CONSTANT(m, VertexAttributeSemantics, TexCoord);
        REG_MODULE_CONSTANT(m, VertexAttributeSemantics, Tangent);
        REG_MODULE_CONSTANT(m, VertexAttributeSemantics, Binormal);
        m.endModule();
    }
}
