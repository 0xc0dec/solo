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

#include "SoloCommon.h"
#include "SoloComponent.h"
#include "SoloNode.h"


namespace solo
{
    class Material;
    class Mesh;
    class Transform;

    class MeshRenderer final: public ComponentBase<MeshRenderer>
    {
    public:
        explicit MeshRenderer(const Node &node);

        void render(RenderContext &context) override final;

        auto getMesh() const -> Mesh*;
        void setMesh(sptr<Mesh> mesh);

        auto getMaterial(uint32_t index) const -> Material*;
        void setMaterial(uint32_t index, sptr<Material> material);
        auto getMaterialCount() const -> uint32_t;

    private:
        sptr<Mesh> mesh;
        Transform *transform = nullptr;
        std::unordered_map<uint32_t, sptr<Material>> materials;
    };

    inline auto MeshRenderer::getMesh() const -> Mesh *
    {
        return mesh.get();
    }

    inline void MeshRenderer::setMesh(sptr<Mesh> mesh)
    {
        this->mesh = mesh;
    }

    inline auto MeshRenderer::getMaterial(uint32_t index) const -> Material *
    {
        return materials.at(index).get();
    }

    inline auto MeshRenderer::getMaterialCount() const -> uint32_t
    {
        return static_cast<uint32_t>(materials.size());
    }
}