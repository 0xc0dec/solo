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
#include "SoloLuaCommon.h"

using namespace solo;


void registerEnums(CppBindModule<LuaBinding> &module)
{
    auto transformSpace = module.beginModule("TransformSpace");
    REGISTER_MODULE_CONSTANT(transformSpace, TransformSpace, Parent);
    REGISTER_MODULE_CONSTANT(transformSpace, TransformSpace, Self);
    REGISTER_MODULE_CONSTANT(transformSpace, TransformSpace, World);
    transformSpace.endModule();

    auto intersection = module.beginModule("PlaneIntersection");
    REGISTER_MODULE_CONSTANT(intersection, PlaneIntersection, Intersecting);
    REGISTER_MODULE_CONSTANT(intersection, PlaneIntersection, Front);
    REGISTER_MODULE_CONSTANT(intersection, PlaneIntersection, Back);
    intersection.endModule();
}