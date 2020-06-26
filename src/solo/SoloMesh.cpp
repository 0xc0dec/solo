/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloMesh.h"
#include "SoloEffect.h"
#include "SoloDevice.h"
#include "SoloFileSystem.h"
#include "SoloJobPool.h"
#include "gl/SoloOpenGLMesh.h"
#include "vk/SoloVulkanMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace solo;

// TODO Remove? This is mostly needed for async loading as we don't want to interact with GPU from the loading thread.
class MeshData
{
public:
    static auto fromFile(Device *device, const str &path, const VertexBufferLayout &bufferLayout) -> sptr<MeshData>
    {
	    // TODO Implement proper io system for assimp to avoid loading file into memory
	    const auto bytes = device->fileSystem()->readBytes(path);
	    
	    Assimp::Importer importer;
	    const auto flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals;
		const auto scene = importer.ReadFileFromMemory(bytes.data(), bytes.size(), flags);
	    panicIf(!scene, "Unable to parse file ", path);

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
	
    static auto fromFileAsync(Device *device, const str &path, const VertexBufferLayout &bufferLayout) -> sptr<AsyncHandle<MeshData>>
    {
	    auto handle = std::make_shared<AsyncHandle<MeshData>>();

	    auto producers = JobBase<MeshData>::Producers{[=]() { return fromFile(device, path, bufferLayout); }};
	    auto consumer = [handle](const vec<sptr<MeshData>> &results) { handle->resolve(results[0]); };
	    device->jobPool()->addJob(std::make_shared<JobBase<MeshData>>(producers, consumer));

	    return handle;
    }

    auto vertexData() const -> const vec<float>& { return vertexData_; }
    auto vertexCount() const -> u32 { return vertexCount_; }
    auto indexesCount() const -> u32 { return indexData_.size(); }
    auto indexData(u32 index) const -> const vec<u32>& { return indexData_.at(index); }
    auto indexElementCount(u32 index) const -> u32 { return indexData_.at(index).size(); }

private:
    vec<float> vertexData_;
    u32 vertexCount_ = 0;
    vec<vec<u32>> indexData_;
};

auto Mesh::empty(Device *device) -> sptr<Mesh>
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
    		panic("Unknown device mode");
    		return nullptr;
    }
}

static auto fromData(Device *device, sptr<MeshData> data, const VertexBufferLayout &bufferLayout) -> sptr<Mesh>
{
    auto mesh = Mesh::empty(device);

    mesh->addVertexBuffer(bufferLayout, data->vertexData(), data->vertexData().size() / bufferLayout.elementCount());

	for (auto part = 0; part < data->indexesCount(); part++)
		mesh->addIndexBuffer(data->indexData(part), data->indexElementCount(part));

    return mesh;
}

auto Mesh::fromFile(Device *device, const str &path, const VertexBufferLayout &bufferLayout) -> sptr<Mesh>
{
    const auto data = MeshData::fromFile(device, path, bufferLayout);
    return fromData(device, data, bufferLayout);
}

auto Mesh::fromFileAsync(Device *device, const str &path, const VertexBufferLayout &bufferLayout)
    -> sptr<AsyncHandle<Mesh>>
{
    auto handle = std::make_shared<AsyncHandle<Mesh>>();

    MeshData::fromFileAsync(device, path, bufferLayout)->done(
        [handle, device, bufferLayout](sptr<MeshData> data)
        {
            handle->resolve(fromData(device, data, bufferLayout));
        });

    return handle;
}

void Mesh::updateMinVertexCount()
{
    constexpr auto max = (std::numeric_limits<u32>::max)();

    minVertexCount_ = max;

    for (const auto &count : vertexCounts_)
        minVertexCount_ = (std::min)(count, minVertexCount_);

    if (minVertexCount_ == max)
        minVertexCount_ = 0;
}

auto Mesh::addVertexBuffer(const VertexBufferLayout &layout, const vec<float> &data, u32 vertexCount) -> u32
{
	layouts_.push_back(layout);
    vertexCounts_.push_back(vertexCount);
	vertexData_.push_back(data); // TODO move
	updateMinVertexCount();
	return static_cast<u32>(vertexCounts_.size() - 1);
}

auto Mesh::addDynamicVertexBuffer(const VertexBufferLayout &layout, const vec<float> &data, u32 vertexCount) -> u32
{
	// TODO No copy-paste
	layouts_.push_back(layout);
    vertexCounts_.push_back(vertexCount);
	vertexData_.push_back(data); // TODO move
	updateMinVertexCount();
	return static_cast<u32>(vertexCounts_.size() - 1);
}

void Mesh::removeVertexBuffer(u32 index)
{
    vertexCounts_.erase(vertexCounts_.begin() + index);
	vertexData_.erase(vertexData_.begin() + index);
    layouts_.erase(layouts_.begin() + index);
	updateMinVertexCount();
}

auto Mesh::addIndexBuffer(const vec<u32> &data, u32 elementCount) -> u32
{
	indexElementCounts_.push_back(elementCount);
	indexData_.push_back(data);
    return static_cast<u32>(indexElementCounts_.size() - 1);
}

void Mesh::removeIndexBuffer(u32 index)
{
	indexElementCounts_.erase(indexElementCounts_.begin() + index);
	indexData_.erase(indexData_.begin() + index);
}
