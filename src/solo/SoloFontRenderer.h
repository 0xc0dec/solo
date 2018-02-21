/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloComponent.h"
#include "SoloVector2.h"
#include "SoloVector3.h"

namespace solo
{
    class Font;
    class Mesh;
    class Material;
    class Transform;
    class Renderer;

    class FontRenderer final: public ComponentBase<FontRenderer>
    {
    public:
        explicit FontRenderer(const Node &node);

        void render() override final;

        void setFont(sptr<Font> font);
        void setText(const str &text);

    private:
        Renderer *renderer = nullptr;
        Transform *transform = nullptr;
        sptr<Font> font;
        sptr<Mesh> mesh;
        sptr<Material> material;
        vec<Vector3> vertices;
        vec<Vector2> uvs;
        vec<u16> indexes;
        str text;

        void rebuildMesh();
        void updateMesh();
    };
}
