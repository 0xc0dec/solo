#pragma once

#include "SoloComponent.h"
#include "SoloVector2.h"
#include "SoloVector3.h"


namespace solo
{
    class Font;
    class Mesh;
    class Material;

    class FontRenderer final: public ComponentBase<FontRenderer>
    {
    public:
        explicit FontRenderer(const Node& node);

        virtual void render(RenderContext& context) override final;

        void setFont(sptr<Font> font);
        void setText(const std::string& text);

    private:
        void rebuildMesh();
        void updateMesh();

        sptr<Font> font;
        sptr<Mesh> mesh;
        sptr<Material> material;
        std::vector<Vector3> vertices;
        std::vector<Vector2> uvs;
        std::vector<uint16_t> indexes;
        std::string text;
    };
}