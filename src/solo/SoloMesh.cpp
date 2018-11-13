/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloMesh.h"
#include "SoloEffect.h"
#include "SoloDevice.h"
#include "SoloJobPool.h"
#include "SoloFileSystem.h"
#include "gl/SoloOpenGLMesh.h"
#include "vk/SoloVulkanMesh.h"
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
    const auto bytes = device->fileSystem()->readBytes(path);
    
    Assimp::Importer importer;
    const auto flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals;
	const auto scene = importer.ReadFileFromMemory(bytes.data(), bytes.size(), flags);
    SL_DEBUG_PANIC(!scene, "Unable to parse file ", path);

    MeshData data;
    vec<vec<u16>> parts;
    u16 indexBase = 0;

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

            for (u32 k = 0; k < bufferLayout.attributeCount(); k++)
            {
                const auto attr = bufferLayout.attribute(k);
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
                part[startIdx] = indexBase + face.mIndices[0];
                part[startIdx + 1] = indexBase + face.mIndices[1];
                part[startIdx + 2] = indexBase + face.mIndices[2];
            }
		}

        indexBase += mesh->mNumFaces * 3;
        data.indexData.emplace_back(std::move(part));
	}

    return std::make_shared<MeshData>(std::move(data));
}

auto Mesh::create(Device *device) -> sptr<Mesh>
{
    switch (device->mode())
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
            SL_DEBUG_PANIC(true, "Unknown device mode");
            return nullptr;
    }
}

auto Mesh::fromFile(Device *device, const str &path, const VertexBufferLayout &bufferLayout) -> sptr<Mesh>
{
    auto data = loadMeshData(device, path, bufferLayout);
    auto mesh = create(device);
    
    mesh->addVertexBuffer(bufferLayout, data->vertexData.data(), data->vertexData.size() / bufferLayout.elementCount());
    
    for (auto &part : data->indexData)
        mesh->addPart(part.data(), part.size());

    return mesh;
}

auto Mesh::fromFileAsync(Device *device, const str &path, const VertexBufferLayout &bufferLayout) -> sptr<AsyncHandle<Mesh>>
{
    auto handle = std::make_shared<AsyncHandle<Mesh>>();

    auto producers = JobBase<MeshData>::Producers{[=]() { return loadMeshData(device, path, bufferLayout); }};
    auto consumer = [handle, device, bufferLayout](const vec<sptr<MeshData>> &results)
    {
        auto data = results[0];
        auto mesh = create(device);
    
        mesh->addVertexBuffer(bufferLayout, data->vertexData.data(), data->vertexData.size() / bufferLayout.elementCount());
    
        for (auto &part : data->indexData)
            mesh->addPart(part.data(), part.size());

        handle->resolve(mesh);
    };

    device->jobPool()->addJob(std::make_shared<JobBase<MeshData>>(producers, consumer));

    return handle;
}
