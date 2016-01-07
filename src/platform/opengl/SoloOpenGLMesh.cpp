#include "SoloOpenGLMesh.h"

using namespace solo;


OpenGLMesh2::OpenGLMesh2()
{
    glGenBuffers(1, &bufferHandle);
    if (!bufferHandle)
        SL_THROW_FMT(EngineException, "Unable to obtain mesh handle");
}


OpenGLMesh2::~OpenGLMesh2()
{
    if (bufferHandle)
        glDeleteBuffers(1, &bufferHandle);
}


GLenum OpenGLMesh2::convertPrimitiveType(PrimitiveType primitiveType)
{
    switch (primitiveType)
    {
    case PrimitiveType::Triangles: return GL_TRIANGLES;
    case PrimitiveType::TriangleStrip: return GL_TRIANGLE_STRIP;
    case PrimitiveType::Lines: return GL_LINES;
    case PrimitiveType::LineStrip: return GL_LINE_STRIP;
    case PrimitiveType::Points: return GL_POINTS;
    default:
        SL_THROW_FMT(EngineException, "Unknown primitive type");
    }
}


GLenum OpenGLMesh2::convertIndexType(MeshIndexFormat indexFormat)
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


OpenGLIndexedMeshPart::OpenGLIndexedMeshPart()
{
    glGenBuffers(1, &bufferHandle);
    if (!bufferHandle)
        SL_THROW_FMT(EngineException, "Unable to obtain indexed mesh part handle");
}


OpenGLIndexedMeshPart::~OpenGLIndexedMeshPart()
{
    if (bufferHandle)
        glDeleteBuffers(1, &bufferHandle);
}


unsigned OpenGLIndexedMeshPart::getElementSize(MeshIndexFormat indexFormat)
{
    switch (indexFormat)
    {
    case MeshIndexFormat::UnsignedByte: return 1;
    case MeshIndexFormat::UnsignedShort: return 2;
    case MeshIndexFormat::UnsignedInt: return 4;
    default:
        SL_THROW_FMT(EngineException, "Unrecognized index format");
    }
}


GLenum OpenGLIndexedMeshPart::convertPrimitiveType(PrimitiveType primitiveType)
{
    // TODO remove copy-paste
    switch (primitiveType)
    {
    case PrimitiveType::Triangles: return GL_TRIANGLES;
    case PrimitiveType::TriangleStrip: return GL_TRIANGLE_STRIP;
    case PrimitiveType::Lines: return GL_LINES;
    case PrimitiveType::LineStrip: return GL_LINE_STRIP;
    case PrimitiveType::Points: return GL_POINTS;
    default:
        SL_THROW_FMT(EngineException, "Unknown primitive type");
    }
}


void OpenGLIndexedMeshPart::resetIndexData(MeshIndexFormat format, float* data, unsigned elementCount, bool dynamic)
{
    if (!data || !elementCount)
        SL_THROW_FMT(EngineException, "Unable to reset index data: no or empty data");

    indexFormat = format;
    elementSize = getElementSize(format);
    bufferElementCount = elementCount;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementSize * elementCount, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void OpenGLIndexedMeshPart::updateIndexData(float* data, unsigned elementCount, unsigned updateFromIndex)
{
    // This check automatically prevents updating a buffer without any data
    if (updateFromIndex + elementCount > bufferElementCount)
        SL_THROW_FMT(EngineException, "Unable to update index data: new data out of bounds");

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferHandle);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, updateFromIndex * elementSize, elementCount * elementSize, data);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void OpenGLMesh2::resetVertexData(const VertexFormat &format, float* data, unsigned elementCount, bool dynamic)
{
    if (!data || !elementCount)
        SL_THROW_FMT(EngineException, "Unable to reset mesh vertex data: no or empty data");

    glBindBuffer(GL_ARRAY_BUFFER, bufferHandle);
    glBufferData(GL_ARRAY_BUFFER, format.getVertexSize() * elementCount, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    vertexFormat = format;
    bufferElementCount = elementCount;
}


void OpenGLMesh2::updateVertexData(float* data, unsigned elementCount, unsigned updateFromIndex)
{
    // This check automatically prevents updating a buffer without any data
    if (updateFromIndex + elementCount > bufferElementCount)
        SL_THROW_FMT(EngineException, "Unable to update mesh vertex data: new data out of bounds");

    glBindBuffer(GL_ARRAY_BUFFER, bufferHandle);
    glBufferSubData(GL_ARRAY_BUFFER, updateFromIndex * vertexFormat.getVertexSize(), elementCount * vertexFormat.getVertexSize(), data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


IndexedMeshPart* OpenGLMesh2::addIndexedPart()
{
    auto part = SL_NEW_SHARED(OpenGLIndexedMeshPart);
    parts.push_back(part);
    return part.get();
}


void OpenGLMesh2::draw()
{
    glDrawArrays(convertPrimitiveType(primitiveType), 0, bufferElementCount);
}


void OpenGLMesh2::drawIndexedPart(unsigned partIndex)
{
    auto part = parts[partIndex];
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, part->getBufferHandle());
    glDrawElements(convertPrimitiveType(part->getPrimitiveType()), part->getElementCount(), convertIndexType(part->getIndexFormat()), nullptr);
}


static GLuint buildElementArrayBuffer(GLuint existingHandle, const std::vector<uint16_t> &elements)
{
    auto handle = existingHandle ? existingHandle : 0;
    if (!handle)
        glGenBuffers(1, &handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(uint16_t), elements.data(), GL_STATIC_DRAW);
    return handle;
}


template<typename TElement>
static GLuint buildArrayBuffer(GLuint existingHandle, const std::vector<TElement> &elements, GLuint elementSize, GLuint vertexAttribIndex, GLenum elementType)
{
    auto handle = existingHandle ? existingHandle : 0;
    if (!handle)
        glGenBuffers(1, &handle);
    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glBufferData(GL_ARRAY_BUFFER, elements.size() * sizeof(TElement), elements.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexAttribIndex);
    glVertexAttribPointer(vertexAttribIndex, elementSize, elementType, GL_FALSE, 0, nullptr);
    return handle;
}


OpenGLMesh::OpenGLMesh():
    Mesh()
{
    glGenVertexArrays(1, &vertexArrayHandle);
    if (!vertexArrayHandle)
        SL_THROW_FMT(EngineException, "Failed to obtain mesh handle");
}


OpenGLMesh::~OpenGLMesh()
{
    if (vertexBufferHandle)
        glDeleteBuffers(1, &vertexBufferHandle);
    if (normalBufferHandle)
        glDeleteBuffers(1, &normalBufferHandle);
    if (uvBufferHandle)
        glDeleteBuffers(1, &uvBufferHandle);
    if (indicesBufferHandle)
        glDeleteBuffers(1, &indicesBufferHandle);
    glDeleteVertexArrays(1, &vertexArrayHandle);
}


void OpenGLMesh::draw()
{
    glBindVertexArray(vertexArrayHandle);
    if (indicesCount > 0)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferHandle);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indicesCount), GL_UNSIGNED_SHORT, nullptr);
    }
    else
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(verticesCount));
}


void OpenGLMesh::setVertices(const std::vector<Vector3> &vertices)
{
    if (!vertices.empty())
    {
        glBindVertexArray(vertexArrayHandle);
        vertexBufferHandle = buildArrayBuffer(vertexBufferHandle, vertices, 3, 0, GL_FLOAT); // binds to the zero vertex attribute
        verticesCount = vertices.size();
    }
}


void OpenGLMesh::setNormals(const std::vector<Vector3> &normals)
{
    if (!normals.empty())
    {
        glBindVertexArray(vertexArrayHandle);
        normalBufferHandle = buildArrayBuffer(normalBufferHandle, normals, 3, 1, GL_FLOAT); // binds to the first vertex attribute
        normalsCount = normals.size();
    }
}


void OpenGLMesh::setUVs(const std::vector<Vector2> &uvs)
{
    if (!uvs.empty())
    {
        glBindVertexArray(vertexArrayHandle);
        uvBufferHandle = buildArrayBuffer(uvBufferHandle, uvs, 2, 2, GL_FLOAT); // binds to the second vertex attribute
        uvsCount = uvs.size();
    }
}


void OpenGLMesh::setIndices(const std::vector<uint16_t> &indices)
{
    if (!indices.empty())
    {
        glBindVertexArray(vertexArrayHandle);
        indicesBufferHandle = buildElementArrayBuffer(indicesBufferHandle, indices);
        indicesCount = indices.size();
    }
}