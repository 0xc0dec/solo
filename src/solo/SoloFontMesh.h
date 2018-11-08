/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
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
        auto mesh() const -> sptr<Mesh> { return mesh_; }

    private:
        Device *device_ = nullptr;
        sptr<Font> font_;
        sptr<Mesh> mesh_;
        vec<Vector3> vertices_;
        vec<Vector2> uvs_;
        vec<u16> indexes_;
        str text_;

        explicit FontMesh(Device *device, sptr<Font> font);

        void rebuildMesh();
        void updateMesh();
    };
}