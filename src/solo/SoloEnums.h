/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include "SoloCommon.h"

namespace solo
{
    enum class KeyCode
    {
        Digit0,
        Digit1,
        Digit2,
        Digit3,
        Digit4,
        Digit5,
        Digit6,
        Digit7,
        Digit8,
        Digit9,
        Q, W, E, R, T, Y, U, I, O, P,
        A, S, D, F, G, H, J, K, L,
        Z, X, C, V, B, N, M,
        Escape,
        LeftArrow,
        RightArrow,
        UpArrow,
        DownArrow,
        Backspace,
        Space,
        Return,
        LeftShift,
        LeftCtrl,
        LeftAlt,
        RightShift,
        RightCtrl,
        RightAlt
        // ...
    };

    enum class MouseButton
    {
        Left,
        Middle,
        Right
    };

    enum class DeviceMode
    {
        OpenGL,
        Vulkan
    };

    enum class FaceCull
    {
        None = 0,
        Front,
        Back
    };

    enum class PolygonMode
    {
        Fill,
        Wireframe,
        Points
    };

    enum class DepthFunction
    {
        Never = 0,
        Less,
        Equal,
        LEqual,
        Greater,
        NotEqual,
        GEqual,
        Always
    };

    enum class BlendFactor
    {
        Zero = 0,
        One,
        SrcColor,
        OneMinusSrcColor,
        DstColor,
        OneMinusDstColor,
        SrcAlpha,
        OneMinusSrcAlpha,
        DstAlpha,
        OneMinusDstAlpha,
        ConstantAlpha,
        OneMinusConstantAlpha,
        SrcAlphaSaturate
    };

    enum class ParameterBinding
    {
        WorldMatrix,
        ViewMatrix,
        ProjectionMatrix,
        WorldViewMatrix,
        ViewProjectionMatrix,
        WorldViewProjectionMatrix,
        InverseTransposedWorldMatrix,
        InverseTransposedWorldViewMatrix,
        CameraWorldPosition
    };

    enum class PrimitiveType
    {
        Triangles,
        TriangleStrip,
        Lines,
        LineStrip,
        Points
    };

    enum class IndexElementSize
    {
        Bits16 = sizeof(u16),
        Bits32 = sizeof(u32)
    };

    enum class TextureWrap
    {
        ClampToEdge = 0,
        ClampToBorder,
        Repeat,
        MirrorRepeat
    };

    enum class TextureFilter
    {
        Nearest = 0,
        Linear
    };

    enum class TextureMipFilter
    {
        None = 0,
        Nearest,
        Linear
    };

    enum class TextureFormat
    {
        Depth24,
        R8,
        RGB8,
        RGBA8,
        RGBA16F
    };

    enum class TextureDataFormat
    {
        Red,
        RGB,
        RGBA
    };

    enum class TransformSpace
    {
        Self,
        Parent,
        World
    };

    enum class VertexAttributeUsage
    {
        Unknown,
        Position,
        Normal,
        TexCoord,
        Tangent,
        Binormal
    };
}