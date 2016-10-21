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

#include "SoloMaterialParameter.h"
#include "SoloEffect.h"
#include "SoloDevice.h"
#include "platform/opengl/SoloOpenGLMaterialParameter.h"
#include "platform/null/SoloNullMaterialParameter.h"

using namespace solo;


auto MaterialParameter::create(Effect* effect, MaterialParameterType type, const char* name) -> sptr<MaterialParameter>
{
    switch (Device::get()->getSetup().mode)
    {
        case DeviceMode::OpenGL:
            return std::make_shared<OpenGLMaterialParameter>(effect, type, name);
        default:
            return std::make_shared<NullMaterialParameter>(type);
    }
}


MaterialParameter::MaterialParameter(MaterialParameterType type):
    type(type)
{
}
