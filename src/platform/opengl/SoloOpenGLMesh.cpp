#include "SoloOpenGLMesh.h"

using namespace solo;


OpenGLMesh::OpenGLMesh(const VertexFormat &vertexFormat):
    Mesh(vertexFormat)
{
    auto elementCount = vertexFormat.getElementCount();
    if (!elementCount)
        return;

    handles.resize(vertexFormat.getStorageCount());
    elementCounts.resize(vertexFormat.getStorageCount());
    
    for (auto i = 0; i < elementCount; ++i)
    {
        auto el = vertexFormat.getElement(i);
        auto handle = handles[el.storageId];
        if (!handle)
        {
            glGenBuffers(1, &handle);
            if (!handle)
                SL_THROW_FMT(EngineException, "Unable to obtain handle for mesh storage ", el.storageId);
            handles[el.storageId] = handle;
            elementCounts[el.storageId] = 0;
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


void OpenGLMesh::resetStorage(unsigned storageId, const float *data, unsigned elementCount, bool dynamic)
{
    // No validations intentionally
    auto handle = handles[storageId];
    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glBufferData(GL_ARRAY_BUFFER, vertexFormat.getVertexSize(storageId) * elementCount, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    elementCounts[storageId] = elementCount;
}


void OpenGLMesh::updateStorage(unsigned storageId, const float *data, unsigned elementCount, unsigned updateFromIndex)
{
    // No validations intentionally
    auto handle = handles[storageId];
    auto vertexSizePerStorage = vertexFormat.getVertexSize(storageId);
    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glBufferSubData(GL_ARRAY_BUFFER, updateFromIndex * vertexSizePerStorage, elementCount * vertexSizePerStorage, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


unsigned OpenGLMesh::addIndex(MeshIndexFormat indexFormat)
{
    GLuint handle = 0;
    glGenBuffers(1, &handle);
    if (!handle)
        SL_THROW_FMT(EngineException, "Unable to obtain mesh index handle");

    indexHandles.push_back(handle);
    indexFormats.push_back(indexFormat);
    indexPrimitiveTypes.push_back(MeshPrimitiveType::Triangles);
    indexElementCounts.push_back(0);

    return indexHandles.size() - 1;
}


void OpenGLMesh::removeIndex(unsigned index)
{
    auto handle = indexHandles[index];
    glDeleteBuffers(1, &handle);

    indexHandles.erase(indexHandles.begin() + index);
    indexFormats.erase(indexFormats.begin() + index);
    indexPrimitiveTypes.erase(indexPrimitiveTypes.begin() + index);
    indexElementCounts.erase(indexElementCounts.begin() + index);
}


void OpenGLMesh::resetIndexData(unsigned index, const void *data, unsigned elementCount, bool dynamic)
{
    // No validations intentionally
    auto handle = indexHandles[index];
    auto format = indexFormats[index];
    auto elementSize = getIndexElementSize(format);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementSize * elementCount, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    indexElementCounts[index] = elementCount;
}


void OpenGLMesh::updateIndexData(unsigned index, const void *data, unsigned elementCount, unsigned updateFromIndex)
{
    // No validations intentionally
    auto handle = indexHandles.at(index);
    auto elementSize = getIndexElementSize(indexFormats[index]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, updateFromIndex * elementSize, elementCount * elementSize, data);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void OpenGLMesh::draw()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDrawArrays(convertPrimitiveType(primitiveType), 0, elementCounts[0]); // TODO really?
}


void OpenGLMesh::drawIndex(unsigned index)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexHandles[index]);
    glDrawElements(convertPrimitiveType(indexPrimitiveTypes[index]), indexElementCounts[index], convertIndexType(indexFormats[index]), nullptr);
}


unsigned OpenGLMesh::getIndexElementSize(MeshIndexFormat indexFormat)
{
    switch (indexFormat)
    {
    case MeshIndexFormat::UnsignedByte:
        return 1;
    case MeshIndexFormat::UnsignedShort:
        return 2;
    case MeshIndexFormat::UnsignedInt:
        return 4;
    default:
        SL_THROW_FMT(EngineException, "Unrecognized index format");
    }
}


GLenum OpenGLMesh::convertPrimitiveType(MeshPrimitiveType primitiveType)
{
    switch (primitiveType)
    {
    case MeshPrimitiveType::Triangles:
        return GL_TRIANGLES;
    case MeshPrimitiveType::TriangleStrip:
        return GL_TRIANGLE_STRIP;
    case MeshPrimitiveType::Lines:
        return GL_LINES;
    case MeshPrimitiveType::LineStrip:
        return GL_LINE_STRIP;
    case MeshPrimitiveType::Points:
        return GL_POINTS;
    default:
        SL_THROW_FMT(EngineException, "Unknown primitive type");
    }
}


GLenum OpenGLMesh::convertIndexType(MeshIndexFormat indexFormat)
{
    switch (indexFormat)
    {
    case MeshIndexFormat::UnsignedByte:
        return GL_UNSIGNED_BYTE;
    case MeshIndexFormat::UnsignedShort:
        return GL_UNSIGNED_SHORT;
    case MeshIndexFormat::UnsignedInt:
        return GL_UNSIGNED_INT;
    default:
        SL_THROW_FMT(EngineException, "Unknown index type");
    }
}