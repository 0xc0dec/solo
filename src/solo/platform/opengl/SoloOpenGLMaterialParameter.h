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

#include "SoloMaterialParameter.h"
#include "SoloOpenGLRenderer.h"


#ifdef SL_OPENGL_RENDERER

namespace solo
{
    class Texture;
    class Renderer;
    class MaterialParameterValue;

    class OpenGLMaterialParameter final: public MaterialParameter
    {
    public:
        OpenGLMaterialParameter(Renderer* renderer, Effect* effect, MaterialParameterType type, const char* name);
        ~OpenGLMaterialParameter();

        void setValue(const void* value) override final;
        void apply(const RenderContext& context) override final;

    private:
        static auto getUniformType(MaterialParameterType type) -> UniformType;

        OpenGLRenderer* renderer = nullptr;
        uint32_t handle = EmptyHandle;

        uptr<MaterialParameterValue> value;
        uint32_t count = 0;
    };
}

#else
#   error OpenGL renderer is not supported on this platform
#endif
