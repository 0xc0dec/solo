/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloObjMeshLoading.h"
#include "SoloMesh.h"
#include "SoloFileSystem.h"
#include "SoloDevice.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include <unordered_map>

using namespace solo;

Vector3 parseVector3(const char *from, const char *to)
{
    char buf[16];
    float result[3];
    size_t bufIdx = 0;
    size_t resultIdx = -1;
    for (; from <= to; ++from)
    {
        const auto c = *from;
        if ((from == to || isspace(c)) && bufIdx > 0)
        {
            buf[bufIdx] = '\0';
            result[++resultIdx] = static_cast<float>(atof(buf));
            bufIdx = 0;
            if (resultIdx == 2)
                break;
        }
        else
            buf[bufIdx++] = c;
    }
    return {result[0], result[1], result[2]};
}

void parseIndexes(const char **from, const char *to, uint32_t **result)
{
    char buf[16];
    size_t bufIdx = 0;
    size_t resultIdx = 0;
    for (; *from <= to; *from += 1)
    {
        const auto c = **from;
        const auto okSym = (c >= '0' && c <= '9') || c == '-' || c == '.';
        if (okSym)
            buf[bufIdx++] = c;

        if (!okSym || *from == to)
        {
            if (bufIdx > 0)
            {
                buf[bufIdx] = '\0';
                *result[resultIdx++] = atoi(buf);
                bufIdx = 0;
            }
            else if (resultIdx > 0)
                result[resultIdx++] = nullptr;
            if (resultIdx > 2)
                break;
        }
    }
}

bool obj::canLoadMesh(const std::string &path)
{
    // TODO Extract helper method (replace in other places too)
    static const std::string ext = ".obj";
    return std::equal(ext.rbegin(), ext.rend(), path.rbegin());
}

auto obj::loadMesh(Device *device, const std::string &path) -> sptr<Mesh>
{
    auto fs = device->getFileSystem();

    // TODO speed up and make more intelligent

    std::vector<Vector3> inputVertices;
    std::vector<Vector3> inputNormals;
    std::vector<Vector2> inputUvs;
    std::vector<uint16_t> currentIndices;
    std::unordered_map<std::string, uint16_t> uniqueIndices;
    std::vector<Vector3> vertexBuffer;
    std::vector<Vector2> uvBuffer;
    std::vector<Vector3> normalBuffer;
    std::vector<std::vector<uint16_t>> indexBuffers;

    auto finishIndex = [&]
    {
        indexBuffers.push_back(std::move(currentIndices));
        currentIndices = std::vector<uint16_t>();
        uniqueIndices.clear();
    };

    fs->iterateLines(path, [&](const std::string & line)
    {
        auto lineSize = line.size();
        if (line[0] == 'v')
        {
            if (line[1] == 'n')
            {
                auto normal = parseVector3(line.c_str() + 3, line.c_str() + lineSize - 3);
                inputNormals.push_back(normal);
            }
            else if (line[1] == 't')
            {
                auto uv = parseVector3(line.c_str() + 3, line.c_str() + lineSize - 3);
                inputUvs.push_back({uv.x, uv.y});
            }
            else
            {
                auto vertex = parseVector3(line.c_str() + 2, line.c_str() + lineSize - 2);
                inputVertices.push_back(vertex);
            }
        }
        else if (line[0] == 'f')
        {
            auto from = line.c_str() + 2;
            auto to = from + lineSize - 3;
            size_t spaceIdx = 1;
            uint32_t vIdx, uvIdx, nIdx;
            uint32_t *idxs[] = {&vIdx, &uvIdx, &nIdx};
            std::string three;
            for (auto i = 0; i < 3; ++i)
            {
                auto nextSpaceIdx = line.find(' ', spaceIdx + 1);
                three.assign(line.substr(spaceIdx + 1, nextSpaceIdx != std::string::npos ? (nextSpaceIdx - spaceIdx - 1) : lineSize - spaceIdx));
                spaceIdx = nextSpaceIdx;
                auto it = uniqueIndices.find(three);
                if (it != uniqueIndices.end())
                {
                    currentIndices.push_back(it->second);
                    from += three.size() + 1;
                }
                else
                {
                    parseIndexes(&from, to, idxs);
                    vertexBuffer.push_back(inputVertices[vIdx - 1]);
                    if (idxs[1])
                        uvBuffer.push_back(inputUvs[uvIdx - 1]);
                    if (idxs[2])
                        normalBuffer.push_back(inputNormals[nIdx - 1]);
                    auto newIndex = static_cast<uint16_t>(vertexBuffer.size() - 1);
                    uniqueIndices[three] = newIndex;
                    currentIndices.push_back(newIndex);
                }
            }
        }
        else if (line[0] == 'o')
        {
            if (!currentIndices.empty())
                finishIndex();
        }
        return true;
    });

    if (!currentIndices.empty())
        finishIndex();

    auto mesh = Mesh::create(device);

    VertexBufferLayout positionLayout;
    positionLayout.addAttribute(3, 0);
    mesh->addVertexBuffer(positionLayout, reinterpret_cast<const float *>(vertexBuffer.data()), static_cast<uint32_t>(vertexBuffer.size()));

    if (!normalBuffer.empty())
    {
        VertexBufferLayout normalLayout;
        normalLayout.addAttribute(3, 1);
        mesh->addVertexBuffer(normalLayout, reinterpret_cast<const float *>(normalBuffer.data()), static_cast<uint32_t>(normalBuffer.size()));
    }

    if (!uvBuffer.empty())
    {
        VertexBufferLayout uvLayout;
        uvLayout.addAttribute(2, 2);
        mesh->addVertexBuffer(uvLayout, reinterpret_cast<const float *>(uvBuffer.data()), static_cast<uint32_t>(uvBuffer.size()));
    }

    for (const auto &indexBuffer : indexBuffers)
        mesh->addPart(reinterpret_cast<const void *>(indexBuffer.data()), static_cast<uint32_t>(indexBuffer.size()));

    mesh->setPrimitiveType(PrimitiveType::Triangles);

    return mesh;
}
