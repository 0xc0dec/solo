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
}