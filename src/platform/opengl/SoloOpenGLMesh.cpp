#include "SoloOpenGLMesh.h"
#include "SoloOpenGLIndexedMeshPart.h"

using namespace solo;


OpenGLMesh::OpenGLMesh(const VertexFormat &vertexFormat):
    Mesh(vertexFormat)
{
    for (auto i = 0; i < vertexFormat.getElementCount(); ++i)
    {
        auto el = vertexFormat.getElement(i);
        if (handles.find(el.storageId) == handles.end())
        {
            GLuint handle = 0;
            glGenBuffers(1, &handle);
            if (!handle)
                SL_THROW_FMT(EngineException, "Unable to obtain mesh buffer handle");
            handles[el.storageId] = handle;
        }
    }
}


OpenGLMesh::~OpenGLMesh()
{
    while (!handles.empty())
    {
        auto it = handles.begin();
        auto handle = it->second;
        glDeleteBuffers(1, &handle);
        handles.erase(it);
    }
}


GLenum OpenGLMesh::convertPrimitiveType(MeshPrimitiveType primitiveType)
{
    switch (primitiveType)
    {
    case MeshPrimitiveType::Triangles: return GL_TRIANGLES;
    case MeshPrimitiveType::TriangleStrip: return GL_TRIANGLE_STRIP;
    case MeshPrimitiveType::Lines: return GL_LINES;
    case MeshPrimitiveType::LineStrip: return GL_LINE_STRIP;
    case MeshPrimitiveType::Points: return GL_POINTS;
    default:
        SL_THROW_FMT(EngineException, "Unknown primitive type");
    }
}


GLenum OpenGLMesh::convertIndexType(MeshIndexFormat indexFormat)
{
    switch (indexFormat)
    {
    case MeshIndexFormat::UnsignedByte: return GL_UNSIGNED_BYTE;
    case MeshIndexFormat::UnsignedShort: return GL_UNSIGNED_SHORT;
    case MeshIndexFormat::UnsignedInt: return GL_UNSIGNED_INT;
    default:
        SL_THROW_FMT(EngineException, "Unknown index type");
    }
}


void OpenGLMesh::resetStorage(unsigned storageId, const float *data, unsigned elementCount, bool dynamic)
{
    if (!data || !elementCount)
        SL_THROW_FMT(EngineException, "Unable to reset mesh storage ", storageId, ": empty or no data");

    auto handle = handles.at(storageId);
    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glBufferData(GL_ARRAY_BUFFER, vertexFormat.getVertexSize(storageId) * elementCount, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    elementCounts[storageId] = bufferElementCount;
}


void OpenGLMesh::updateStorage(unsigned storageId, const float *data, unsigned elementCount, unsigned updateFromIndex)
{
    if (!data || !elementCount)
        SL_THROW_FMT(EngineException, "Unable to update mesh storage ", storageId, ": empty or no data");

    // This check automatically prevents updating a buffer without any data
    if (updateFromIndex + elementCount > elementCounts.at(storageId))
        SL_THROW_FMT(EngineException, "Unable to update mesh storage ", storageId, ": new data out of bounds");

    auto handle = handles.at(storageId);
    auto vertexSizePerStorage = vertexFormat.getVertexSize(storageId);
    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glBufferSubData(GL_ARRAY_BUFFER, updateFromIndex * vertexSizePerStorage, elementCount * vertexSizePerStorage, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


IndexedMeshPart* OpenGLMesh::addPart(MeshIndexFormat indexFormat)
{
    auto part = SL_NEW_SHARED(OpenGLIndexedMeshPart, indexFormat);
    parts.push_back(part);
    return part.get();
}


IndexedMeshPart *OpenGLMesh::getPart(unsigned index) const
{
    return parts.at(index).get();
}


void OpenGLMesh::draw()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDrawArrays(convertPrimitiveType(primitiveType), 0, bufferElementCount);
}


void OpenGLMesh::drawPart(unsigned partIndex)
{
    auto part = parts.at(partIndex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, part->getBufferHandle());
    glDrawElements(convertPrimitiveType(part->getPrimitiveType()), part->getElementCount(), convertIndexType(part->getIndexFormat()), nullptr);
}
