/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloLuaCommon.h"
#include "SoloFont.h"
#include "SoloFontMesh.h"

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
    REG_METHOD(font, Font, atlas);
    REG_METHOD(font, Font, glyphInfo);
    font.endClass();
}

static void registerFontMesh(CppBindModule<LuaBinding> &module)
{
    auto fm = BEGIN_CLASS(module, FontMesh);
    REG_STATIC_METHOD(fm, FontMesh, create);
    REG_METHOD(fm, FontMesh, setText);
    REG_METHOD(fm, FontMesh, mesh);
    fm.endClass();
}

void registerFontApi(CppBindModule<LuaBinding> &module)
{
    registerFont(module);
    registerFontMesh(module);
}
