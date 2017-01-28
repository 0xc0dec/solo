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
#include "SoloCubeTexture.h"
#include "SoloLuaCommon.h"

using namespace solo;


static void setRectTextureData(RectTexture *tex, TextureFormat format, const std::vector<uint8_t> &data, uint32_t width, uint32_t height)
{
    tex->setData(format, data.data(), width, height);
}


static void setCubeTextureData(CubeTexture *tex, CubeTextureFace face, TextureFormat format, const std::vector<uint8_t> &data, uint32_t width, uint32_t height)
{
    tex->setData(face, format, data.data(), width, height);
}


void registerTexture(CppBindModule<LuaBinding> &module)
{
    auto tex = module.beginClass<Texture>("Texture");
    
    REG_METHOD(tex, Texture, generateMipmaps);

    REG_METHOD(tex, Texture, getHorizontalWrapping);
    REG_METHOD(tex, Texture, getVerticalWrapping);

    REG_METHOD(tex, Texture, setWrapping);
    REG_METHOD(tex, Texture, setHorizontalWrapping);
    REG_METHOD(tex, Texture, setVerticalWrapping);

    REG_METHOD(tex, Texture, getMinFiltering);
    REG_METHOD(tex, Texture, getMagFiltering);

    REG_METHOD(tex, Texture, setFiltering);
    REG_METHOD(tex, Texture, setMinFiltering);
    REG_METHOD(tex, Texture, setMagFiltering);

    REG_METHOD(tex, Texture, getAnisotropyLevel);
    REG_METHOD(tex, Texture, setAnisotropyLevel);
    tex.endClass();

    auto rectTex = module.beginExtendClass<RectTexture, Texture>("RectTexture");
    REG_STATIC_METHOD(rectTex, RectTexture, create);
    REG_FREE_FUNC_AS_METHOD_RENAMED(rectTex, setRectTextureData, "setData");
    REG_METHOD(rectTex, RectTexture, getSize);
    rectTex.endClass();

    auto cubeTex = module.beginExtendClass<CubeTexture, Texture>("CubeTexture");
    REG_STATIC_METHOD(cubeTex, CubeTexture, create);
    REG_FREE_FUNC_AS_METHOD_RENAMED(cubeTex, setCubeTextureData, "setData");
    REG_METHOD(cubeTex, CubeTexture, getDepthWrapping);
    REG_METHOD(cubeTex, CubeTexture, setDepthWrapping);
    cubeTex.endClass();
}