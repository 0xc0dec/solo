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
#include "SoloVertexFormat.h"
#include "SoloRenderer.h"
#include "SoloVector2.h"
#include "SoloVector3.h"


namespace solo
{
    class Effect;
    class Device;

    enum class MeshPrefab
    {
        Quad,
        Cube
    };

    struct MeshData
    {
        std::vector<Vector3> vertices;
        std::vector<Vector2> uvs;
        std::vector<Vector3> normals;
        std::vector<std::vector<uint16_t>> indices;
    };

    class Mesh final
    {
    public:
        static auto create() -> sptr<Mesh>;
        static auto create(MeshPrefab prefab) -> sptr<Mesh>;
        static auto create(MeshData* data) -> sptr<Mesh>;

        SL_DISABLE_COPY_AND_MOVE(Mesh)
        ~Mesh();

        auto addVertexBuffer(const VertexBufferLayout& layout, const float* data, uint32_t vertexCount) -> uint32_t;
        auto addDynamicVertexBuffer(const VertexBufferLayout& layout, const float* data, uint32_t vertexCount) -> uint32_t;
        void updateDynamicVertexBuffer(uint32_t index, uint32_t vertexOffset, const float* data, uint32_t vertexCount);
        void removeVertexBuffer(uint32_t index);

        auto addPart(const void* indexData, uint32_t indexElementCount) -> uint32_t;
        void removePart(uint32_t index);
        auto getPartCount() const -> uint32_t;

        void draw(Effect* effect);
        void drawPart(Effect* effect, uint32_t part);

        void setPrimitiveType(PrimitiveType type);
        auto getPrimitiveType() const -> PrimitiveType;

    private:
        Mesh(Device* device);
        explicit Mesh(Device* device, MeshPrefab prefab);
        explicit Mesh(Device* device, MeshData* data);

        void initQuadMesh();
        void initCubeMesh();

        auto addVertexBuffer(VertexBufferHandle bufferHandle, const VertexBufferLayout& layout, uint32_t vertexCount) -> uint32_t;

        void rebuildEffectBinding(Effect* effect);
        void recalculateMinVertexCount();

        Renderer* renderer;
        Effect* lastEffect = nullptr;

        PrimitiveType primitiveType = PrimitiveType::Triangles;
        std::vector<VertexBufferHandle> vertexBuffers;
        std::vector<IndexBufferHandle> indexBuffers;
        std::vector<uint32_t> vertexCounts;
        std::vector<uint32_t> vertexSizes;
        uint32_t minVertexCount = 0;
        VertexProgramBindingHandle programBinding;
    };

    inline void Mesh::setPrimitiveType(PrimitiveType type)
    {
        primitiveType = type;
    }

    inline auto Mesh::getPrimitiveType() const -> PrimitiveType
    {
        return primitiveType;
    }

    inline auto Mesh::getPartCount() const -> uint32_t
    {
        return static_cast<uint32_t>(indexBuffers.size());
    }
}
