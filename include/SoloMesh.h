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

    class Mesh
    {
    public:
        static auto create() -> sptr<Mesh>;
        static auto create(MeshPrefab prefab) -> sptr<Mesh>;
        static auto create(MeshData *data) -> sptr<Mesh>;

        SL_DISABLE_COPY_AND_MOVE(Mesh)
        virtual ~Mesh() {}

        virtual auto addVertexBuffer(const VertexBufferLayout &layout, const float *data, uint32_t vertexCount) -> uint32_t = 0;
        virtual auto addDynamicVertexBuffer(const VertexBufferLayout &layout, const float *data, uint32_t vertexCount) -> uint32_t = 0;
        virtual void updateDynamicVertexBuffer(uint32_t index, uint32_t vertexOffset, const float *data, uint32_t vertexCount) = 0;
        virtual void removeVertexBuffer(uint32_t index) = 0;

        virtual auto addPart(const void *indexData, uint32_t indexElementCount) -> uint32_t = 0;
        virtual void removePart(uint32_t index) = 0;
        virtual auto getPartCount() const -> uint32_t = 0;

        virtual void draw(Effect *effect) = 0;
        virtual void drawPart(Effect *effect, uint32_t part) = 0;

        virtual auto getPrimitiveType() const -> PrimitiveType = 0;
        virtual void setPrimitiveType(PrimitiveType type) = 0;

    protected:
        Mesh() {}

        void initQuadMesh();
        void initCubeMesh();
    };
}
