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

#pragma once

#include "SoloCommon.h"


namespace solo
{
    class Effect;
    struct RenderContext;

    enum class MaterialParameterType
    {
        Float,
        FloatArray,
        Vector2,
        Vector2Array,
        Vector3,
        Vector3Array,
        Vector4,
        Vector4Array,
        Matrix,
        MatrixArray,
        Texture,
        // Auto-binding values
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

    class MaterialParameter
    {
    public:
        static auto create(Effect* effect, MaterialParameterType type, const char* name) -> sptr<MaterialParameter>;

        SL_DISABLE_COPY_AND_MOVE(MaterialParameter)
        virtual ~MaterialParameter() {}

        auto getType() const -> MaterialParameterType;

        virtual void setValue(const void* value) = 0;
        virtual void apply(const RenderContext& context) = 0;

    protected:
        MaterialParameter(MaterialParameterType type);

        MaterialParameterType type;
    };

    inline auto MaterialParameter::getType() const -> MaterialParameterType
    {
        return type;
    }
}