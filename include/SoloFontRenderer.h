/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloComponent.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include <vector>


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

        void render(const RenderContext &context) override final;

        void setFont(sptr<Font> font);
        void setText(const std::string &text);

    private:
        Renderer *renderer = nullptr;
        Transform *transform = nullptr;
        sptr<Font> font;
        sptr<Mesh> mesh;
        sptr<Material> material;
        std::vector<Vector3> vertices;
        std::vector<Vector2> uvs;
        std::vector<uint16_t> indexes;
        std::string text;

        void rebuildMesh();
        void updateMesh();
    };
}
