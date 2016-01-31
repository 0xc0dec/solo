#include "SoloOpenGLMesh.h"
#include "SoloOpenGLHelper.h"

using namespace solo;


OpenGLMesh::OpenGLMesh(const VertexFormat& vertexFormat):
    Mesh(vertexFormat)
{
    auto elementCount = vertexFormat.getElementCount();
    if (!elementCount)
        return;

    handles.resize(vertexFormat.getSlotCount());
    elementCounts.resize(vertexFormat.getSlotCount());

    for (auto i = 0; i < elementCount; ++i)
    {
        auto el = vertexFormat.getElement(i);
        auto handle = handles[el.slot];
        if (!handle)
        {
            glGenBuffers(1, &handle);
            if (!handle)
                SL_THROW_FMT(EngineException, "Unable to obtain handle for mesh slot ", el.slot);
            handles[el.slot] = handle;
            elementCounts[el.slot] = 0;
        }
    }
}


OpenGLMesh::~OpenGLMesh()
{
    for (auto i = 0; i < handles.size(); ++i)
    {
        auto handle = handles[i];
        glDeleteBuffers(1, &handle);
    }
    for (auto i = 0; i < indexHandles.size(); ++i)
    {
        auto handle = indexHandles[i];
        glDeleteBuffers(1, &handle);
    }
}


void OpenGLMesh::resetData(int slot, const float* data, int elementCount, bool dynamic)
{
    // No validations intentionally
    auto handle = handles[slot];
    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glBufferData(GL_ARRAY_BUFFER, vertexFormat.getVertexSize(slot) * elementCount, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    elementCounts[slot] = elementCount;
}


void OpenGLMesh::updateData(int slot, const float* data, int elementCount, int updateFromIndex)
{
    // No validations intentionally
    auto handle = handles[slot];
    auto vertexSizePerSlot = vertexFormat.getVertexSize(slot);
    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glBufferSubData(GL_ARRAY_BUFFER, updateFromIndex * vertexSizePerSlot, elementCount * vertexSizePerSlot, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


int OpenGLMesh::addIndex(MeshIndexFormat indexFormat)
{
    GLuint handle = 0;
    glGenBuffers(1, &handle);
    if (!handle)
        SL_THROW_FMT(EngineException, "Unable to obtain mesh index handle");

    indexHandles.push_back(handle);
    indexFormats.push_back(indexFormat);
    indexPrimitiveTypes.push_back(MeshPrimitiveType::Triangles);
    indexElementCounts.push_back(0);

    return static_cast<int>(indexHandles.size() - 1);
}


void OpenGLMesh::removeIndex(int index)
{
    auto handle = indexHandles[index];
    glDeleteBuffers(1, &handle);

    indexHandles.erase(indexHandles.begin() + index);
    indexFormats.erase(indexFormats.begin() + index);
    indexPrimitiveTypes.erase(indexPrimitiveTypes.begin() + index);
    indexElementCounts.erase(indexElementCounts.begin() + index);
}


void OpenGLMesh::resetIndexData(int index, const void* data, int elementCount, bool dynamic)
{
    // No validations intentionally
    auto handle = indexHandles[index];
    auto format = indexFormats[index];
    auto elementSize = OpenGLHelper::getMeshIndexElementSize(format);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementSize * elementCount, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    indexElementCounts[index] = elementCount;
}


void OpenGLMesh::updateIndexData(int index, const void* data, int elementCount, int updateFromIndex)
{
    // No validations intentionally
    auto handle = indexHandles.at(index);
    auto elementSize = OpenGLHelper::getMeshIndexElementSize(indexFormats[index]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, updateFromIndex * elementSize, elementCount * elementSize, data);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void OpenGLMesh::draw()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDrawArrays(OpenGLHelper::convertMeshPrimitiveType(primitiveType), 0, elementCounts[0]); // TODO really?
}


void OpenGLMesh::drawIndex(int index)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexHandles[index]);
    glDrawElements(OpenGLHelper::convertMeshPrimitiveType(indexPrimitiveTypes[index]),
                   indexElementCounts[index], OpenGLHelper::convertMeshIndexType(indexFormats[index]), nullptr);
}
