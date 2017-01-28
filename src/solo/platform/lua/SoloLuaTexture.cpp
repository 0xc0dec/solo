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

#include "SoloTexture.h"
#include "SoloRectTexture.h"
#include "SoloLuaCommon.h"

using namespace solo;


void registerTexture(CppBindModule<LuaBinding> &module)
{
    auto tex = module.beginClass<Texture>("Texture");
    REGISTER_METHOD(tex, Texture, getHorizontalWrapping);
    REGISTER_METHOD(tex, Texture, setHorizontalWrapping);
    REGISTER_METHOD(tex, Texture, getVerticalWrapping);
    REGISTER_METHOD(tex, Texture, setVerticalWrapping);
    REGISTER_METHOD(tex, Texture, setWrapping);
    REGISTER_METHOD(tex, Texture, getMinFiltering);
    REGISTER_METHOD(tex, Texture, setMinFiltering);
    REGISTER_METHOD(tex, Texture, getMagFiltering);
    REGISTER_METHOD(tex, Texture, setMagFiltering);
    REGISTER_METHOD(tex, Texture, setFiltering);
    REGISTER_METHOD(tex, Texture, getAnisotropyLevel);
    REGISTER_METHOD(tex, Texture, setAnisotropyLevel);
    tex.endClass();

    auto rectTex = module.beginExtendClass<RectTexture, Texture>("RectTexture");
    REGISTER_STATIC_METHOD(rectTex, RectTexture, create);
    // TODO
    rectTex.endClass();
}