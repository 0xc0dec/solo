/*
    Copyright (c) Aleksey Fedotov
    MIT license
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
    REG_METHOD_NULLABLE_1ST_ARG(r, FontRenderer, setFont, sptr<Font>);
    REG_METHOD(r, FontRenderer, setText);
    r.endClass();
}

void registerFontApi(CppBindModule<LuaBinding> &module)
{
    registerFont(module);
    registerFontRenderer(module);
}
