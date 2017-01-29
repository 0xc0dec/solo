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

#include "SoloLuaCommon.h"
#include "SoloFont.h"
#include "SoloFontRenderer.h"

using namespace solo;


static auto createFont(Device *device, const std::vector<uint8_t> &fontData, uint32_t size, uint32_t atlasWidth, uint32_t atlasHeight, 
    uint32_t firstChar, uint32_t charCount, uint32_t oversampleX, uint32_t oversampleY) -> sptr<Font>
{
    // TODO remove const_cast, or maybe the need for this wrapper altogether
    return Font::create(device, const_cast<uint8_t*>(fontData.data()), size, atlasWidth, atlasHeight, firstChar, charCount, oversampleX, oversampleY);
}


static void registerFont(CppBindModule<LuaBinding> &module)
{
    auto gi = BEGIN_CLASS(module, GlyphInfo);
    REG_VARIABLE(gi, GlyphInfo, positions);
    REG_VARIABLE(gi, GlyphInfo, uvs);
    REG_VARIABLE(gi, GlyphInfo, offsetX);
    REG_VARIABLE(gi, GlyphInfo, offsetY);
    gi.endClass();

    auto font = BEGIN_CLASS(module, Font);
    font.addStaticFunction("create", createFont);
    REG_METHOD(font, Font, getAtlas);
    REG_METHOD(font, Font, getGlyphInfo);
    font.endClass();
}


static void registerFontRenderer(CppBindModule<LuaBinding> &module)
{
    auto r = BEGIN_CLASS_EXTEND(module, FontRenderer, Component);
    REG_METHOD(r, FontRenderer, setFont);
    REG_METHOD(r, FontRenderer, setText);
    r.endClass();
}


void registerFontApi(CppBindModule<LuaBinding> &module)
{
    registerFont(module);
    registerFontRenderer(module);
}
