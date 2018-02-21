/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloMesh.h"
#include "SoloEffect.h"
#include "SoloDevice.h"
#include "SoloJobPool.h"
#include "SoloFileSystem.h"
#include "gl/SoloOpenGLMesh.h"
#include "vk/SoloVulkanMesh.h"
#include "null/SoloNullMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace solo;

struct MeshData
{
    vec<float> vertexData;
    u32 vertexCount = 0;
    vec<vec<u16>> indexData;
};

static auto loadMeshData(Device *device, const str &path, const VertexBufferLayout &bufferLayout) -> sptr<MeshData>
{
    // TODO Implement proper io system for assimp to avoid loading file into memory
    const auto bytes = device->getFileSystem()->readBytes(path);
    
    Assimp::Importer importer;
    const auto flags = aiProcess_Triangulate | aiProcess_PreTransformVertices | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals;
	const auto scene = importer.ReadFileFromMemory(bytes.data(), bytes.size(), flags);
    panicIf(!scene, SL_FMT("Unsupported mesh file ", path));

    MeshData data;
    vec<vec<u16>> parts;

    // TODO resize vertices beforehand
    for (u32 i = 0; i < scene->mNumMeshes; i++)
	{
		const aiMesh* mesh = scene->mMeshes[i];
		const aiVector3D zeroVec(0.0f, 0.0f, 0.0f);

		for (u32 j = 0; j < mesh->mNumVertices; j++)
		{
			const auto pos = &mesh->mVertices[j];
			const auto normal = &mesh->mNormals[j];
			const auto texCoord = mesh->HasTextureCoords(0) ? &mesh->mTextureCoords[0][j] : &zeroVec;
			const auto tangent = mesh->HasTangentsAndBitangents() ? &mesh->mTangents[j] : &zeroVec;
			const auto biTangent = mesh->HasTangentsAndBitangents() ? &mesh->mBitangents[j] : &zeroVec;

            data.vertexCount++;

            for (u32 k = 0; k < bufferLayout.getAttributeCount(); k++)
            {
                const auto attr = bufferLayout.getAttribute(k);
                switch (attr.semantics)
                {
                    case VertexAttributeSemantics::Position:
                        data.vertexData.push_back(pos->x);
			            data.vertexData.push_back(pos->y);
			            data.vertexData.push_back(pos->z);
                        break;
                    case VertexAttributeSemantics::Normal:
                        data.vertexData.push_back(normal->x);
			            data.vertexData.push_back(normal->y);
			            data.vertexData.push_back(normal->z);
                        break;
                    case VertexAttributeSemantics::TexCoord:
                        data.vertexData.push_back(texCoord->x);
                        data.vertexData.push_back(texCoord->y);
                        break;
                    case VertexAttributeSemantics::Tangent:
                        data.vertexData.push_back(tangent->x);
			            data.vertexData.push_back(tangent->y);
			            data.vertexData.push_back(tangent->z);
                        break;
                    case VertexAttributeSemantics::Binormal:
                        data.vertexData.push_back(biTangent->x);
			            data.vertexData.push_back(biTangent->y);
			            data.vertexData.push_back(biTangent->z);
                        break;
                    default: break;
                }
            }
		}

        vec<u16> part;
        part.resize(mesh->mNumFaces * 3);
		for (u32 j = 0; j < mesh->mNumFaces; j++)
		{
			const auto &face = mesh->mFaces[j];
			if (face.mNumIndices == 3)
            {
                const auto startIdx = j * 3;
                part[startIdx] = face.mIndices[0];
                part[startIdx + 1] = face.mIndices[1];
                part[startIdx + 2] = face.mIndices[2];
            }
		}

        data.indexData.emplace_back(std::move(part));
	}

    return std::make_shared<MeshData>(std::move(data));
}

auto Mesh::create(Device *device) -> sptr<Mesh>
{
    switch (device->getMode())
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<OpenGLMesh>();
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return std::make_shared<VulkanMesh>(device);
#endif
        default:
            return std::make_shared<NullMesh>();
    }
}

auto Mesh::createFromPrefab(Device *device, MeshPrefab prefab) -> sptr<Mesh>
{
    auto mesh = create(device);

    switch (prefab)
    {
        case MeshPrefab::Quad:
            mesh->initAsQuadMesh();
            break;
        case MeshPrefab::Cube:
            mesh->initAsCubeMesh();
            break;
        default:
            panic("Unknown mesh prefab type");
            break;
    }

    return mesh;
}

auto Mesh::loadFromFile(Device *device, const str &path, const VertexBufferLayout &bufferLayout) -> sptr<Mesh>
{
    auto data = loadMeshData(device, path, bufferLayout);
    auto mesh = create(device);
    
    mesh->addVertexBuffer(bufferLayout, data->vertexData.data(), data->vertexData.size() / bufferLayout.getElementCount());
    
    for (auto &part : data->indexData)
        mesh->addPart(part.data(), part.size());

    return mesh;
}

auto Mesh::loadFromFileAsync(Device *device, const str &path, const VertexBufferLayout &bufferLayout) -> sptr<AsyncHandle<Mesh>>
{
    auto handle = std::make_shared<AsyncHandle<Mesh>>();

    auto producers = JobBase<MeshData>::Producers{[=]() { return loadMeshData(device, path, bufferLayout); }};
    auto consumer = [handle, device, bufferLayout](const vec<sptr<MeshData>> &results)
    {
        auto data = results[0];
        auto mesh = create(device);
    
        mesh->addVertexBuffer(bufferLayout, data->vertexData.data(), data->vertexData.size() / bufferLayout.getElementCount());
    
        for (auto &part : data->indexData)
            mesh->addPart(part.data(), part.size());

        handle->resolve(mesh);
    };

    device->getJobPool()->addJob(std::make_shared<JobBase<MeshData>>(producers, consumer));

    return handle;
}

void Mesh::initAsQuadMesh()
{
    float vertices[] =
    {
        -1, -1, 0, 0, 0,
        -1,  1, 0, 0, 1,
         1,  1, 0, 1, 1,
        -1, -1, 0, 0, 0,
         1,  1, 0, 1, 1,
         1, -1, 0, 1, 0
    };

    VertexBufferLayout layout;
    layout.addSemanticAttribute(VertexAttributeSemantics::Position);
    layout.addSemanticAttribute(VertexAttributeSemantics::TexCoord);

    addVertexBuffer(layout, vertices, 6);
    setPrimitiveType(PrimitiveType::Triangles);
}

void Mesh::initAsCubeMesh()
{
    float vertices[] =
    {
        -1, -1,  1,  0,  0,  1, 0, 0,    -1,  1,  1,  0,  0,  1, 0, 1,      1,  1,  1,  0,  0,  1, 1, 1,     1, -1,  1,  0,  0,  1, 1, 0,
        -1, -1, -1, -1,  0,  0, 0, 0,    -1,  1, -1, -1,  0,  0, 0, 1,     -1,  1,  1, -1,  0,  0, 1, 1,    -1, -1,  1, -1,  0,  0, 1, 0,
         1, -1, -1,  0,  0, -1, 0, 0,     1,  1, -1,  0,  0, -1, 0, 1,     -1,  1, -1,  0,  0, -1, 1, 1,    -1, -1, -1,  0,  0, -1, 1, 0,
         1, -1,  1,  1,  0,  0, 0, 0,     1,  1,  1,  1,  0,  0, 0, 1,      1,  1, -1,  1,  0,  0, 1, 1,     1, -1, -1,  1,  0,  0, 1, 0,
        -1,  1,  1,  0,  1,  0, 0, 0,    -1,  1, -1,  0,  1,  0, 0, 1,      1,  1, -1,  0,  1,  0, 1, 1,     1,  1,  1,  0,  1,  0, 1, 0,
        -1, -1, -1,  0, -1,  0, 0, 0,    -1, -1,  1,  0, -1,  0, 0, 1,      1, -1,  1,  0, -1,  0, 1, 1,     1, -1, -1,  0, -1,  0, 1, 0
    };

    u16 indices[] =
    {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7,
        8, 9, 10,
        8, 10, 11,
        12, 13, 14,
        12, 14, 15,
        16, 17, 18,
        16, 18, 19,
        20, 21, 22,
        20, 22, 23
    };

    VertexBufferLayout layout;
    layout.addSemanticAttribute(VertexAttributeSemantics::Position);
    layout.addSemanticAttribute(VertexAttributeSemantics::Normal);
    layout.addSemanticAttribute(VertexAttributeSemantics::TexCoord);

    addVertexBuffer(layout, vertices, 24);
    addPart(indices, 36);
    setPrimitiveType(PrimitiveType::Triangles);
}
