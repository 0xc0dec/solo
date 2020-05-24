/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloMeshData.h"
#include "SoloDevice.h"
#include "SoloJobPool.h"
#include "SoloFileSystem.h"
#include "SoloMesh.h"
#include "SoloVertexBufferLayout.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace solo;

auto MeshData::fromFile(Device *device, const str &path, const VertexBufferLayout &bufferLayout) -> sptr<MeshData>
{
    // TODO Implement proper io system for assimp to avoid loading file into memory
    const auto bytes = device->fileSystem()->readBytes(path);
    
    Assimp::Importer importer;
    const auto flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals;
	const auto scene = importer.ReadFileFromMemory(bytes.data(), bytes.size(), flags);
    SL_DEBUG_PANIC(!scene, "Unable to parse file ", path);

    auto data = std::make_shared<MeshData>();
    u32 indexBase = 0;

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

            data->vertexCount_++;

            for (u32 k = 0; k < bufferLayout.attributeCount(); k++)
            {
                const auto attr = bufferLayout.attribute(k);
                switch (attr.usage)
                {
                    case VertexAttributeUsage::Position:
                        data->vertexData_.push_back(pos->x);
			            data->vertexData_.push_back(pos->y);
			            data->vertexData_.push_back(pos->z);
                        break;
                    case VertexAttributeUsage::Normal:
                        data->vertexData_.push_back(normal->x);
			            data->vertexData_.push_back(normal->y);
			            data->vertexData_.push_back(normal->z);
                        break;
                    case VertexAttributeUsage::TexCoord:
                        data->vertexData_.push_back(texCoord->x);
                        data->vertexData_.push_back(texCoord->y);
                        break;
                    case VertexAttributeUsage::Tangent:
                        data->vertexData_.push_back(tangent->x);
			            data->vertexData_.push_back(tangent->y);
			            data->vertexData_.push_back(tangent->z);
                        break;
                    case VertexAttributeUsage::Binormal:
                        data->vertexData_.push_back(biTangent->x);
			            data->vertexData_.push_back(biTangent->y);
			            data->vertexData_.push_back(biTangent->z);
                        break;
                    default: break;
                }
            }
		}

        vec<u32> part;
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
        data->indexData_.emplace_back(std::move(part));
	}

    return data;
}

auto MeshData::fromFileAsync(Device *device, const str &path, const VertexBufferLayout &bufferLayout) -> sptr<AsyncHandle<MeshData>>
{
    auto handle = std::make_shared<AsyncHandle<MeshData>>();

    auto producers = JobBase<MeshData>::Producers{[=]() { return fromFile(device, path, bufferLayout); }};
    auto consumer = [handle](const vec<sptr<MeshData>> &results) { handle->resolve(results[0]); };
    device->jobPool()->addJob(std::make_shared<JobBase<MeshData>>(producers, consumer));

    return handle;
}
