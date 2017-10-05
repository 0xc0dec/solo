/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloLuaCommon.h"
#include "SoloFont.h"
#include "SoloFontRenderer.h"

using namespace solo;

static void registerFont(CppBindModule<LuaBinding> &module)
{
    auto gi = BEGIN_CLASS(module, GlyphInfo);
    REG_CTOR(gi);
    REG_FIELD(gi, GlyphInfo, positions);
    REG_FIELD(gi, GlyphInfo, uvs);
    REG_FIELD(gi, GlyphInfo, offsetX);
    REG_FIELD(gi, GlyphInfo, offsetY);
    gi.endClass();

    auto font = BEGIN_CLASS(module, Font);
    REG_STATIC_METHOD(font, Font, loadFromFile);
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
