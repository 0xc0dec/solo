/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloLuaCommon.h"
#include "SoloFont.h"
#include "SoloFontMesh.h"

using namespace solo;

static void registerFont(CppBindModule<LuaBinding> &module) {
    auto gi = BEGIN_CLASS(module, GlyphInfo);
    REG_CTOR(gi);
    REG_FIELD(gi, GlyphInfo, positions);
    REG_FIELD(gi, GlyphInfo, uvs);
    REG_FIELD(gi, GlyphInfo, offsetX);
    REG_FIELD(gi, GlyphInfo, offsetY);
    gi.endClass();

    auto binding = BEGIN_CLASS(module, Font);
    REG_STATIC_METHOD(binding, Font, loadFromFile);
    REG_METHOD(binding, Font, atlas);
    REG_METHOD(binding, Font, glyphInfo);
    REG_PTR_EQUALITY(binding, Font);
    binding.endClass();
}

static void registerFontMesh(CppBindModule<LuaBinding> &module) {
    auto binding = BEGIN_CLASS(module, FontMesh);
    REG_STATIC_METHOD(binding, FontMesh, fromFont);
    REG_METHOD(binding, FontMesh, setText);
    REG_METHOD(binding, FontMesh, mesh);
    REG_PTR_EQUALITY(binding, FontMesh);
    binding.endClass();
}

void registerFontApi(CppBindModule<LuaBinding> &module) {
    registerFont(module);
    registerFontMesh(module);
}
