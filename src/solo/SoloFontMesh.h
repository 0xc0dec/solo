/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include "SoloVector2.h"
#include "SoloVector3.h"

namespace solo
{
    class Font;
    class Device;
    class Mesh;

    class FontMesh
    {
    public:
        static auto create(Device *device, sptr<Font> font) -> sptr<FontMesh>;

        void setText(const str &text);
        auto getMesh() const -> sptr<Mesh> { return mesh; }

    private:
        Device *device = nullptr;
        sptr<Font> font;
        sptr<Mesh> mesh;
        vec<Vector3> vertices;
        vec<Vector2> uvs;
        vec<u16> indexes;
        str text;

        explicit FontMesh(Device *device, sptr<Font> font);

        void rebuildMesh();
        void updateMesh();
    };
}