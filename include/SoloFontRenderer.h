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
        explicit FontRenderer(const Node &node);

        void render(RenderContext &context) override final;

        void setFont(sptr<Font> font);
        void setText(const std::string &text);

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