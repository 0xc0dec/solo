#include "SoloOpenGLIndexedMeshPart.h"

using namespace solo;


OpenGLIndexedMeshPart::OpenGLIndexedMeshPart(MeshIndexFormat indexFormat):
    IndexedMeshPart(indexFormat),
    elementSize(getElementSize(indexFormat))
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


void OpenGLIndexedMeshPart::resetIndexData(const void *data, unsigned elementCount, bool dynamic)
{
    if (!data || !elementCount)
        SL_THROW_FMT(EngineException, "Unable to reset index data: empty or no data");

    bufferElementCount = elementCount;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementSize * elementCount, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void OpenGLIndexedMeshPart::updateIndexData(const void *data, unsigned elementCount, unsigned updateFromIndex)
{
    if (!data || !elementCount)
        SL_THROW_FMT(EngineException, "Unable to update index data: empty or no data");

    // This check automatically prevents updating a buffer without any data
    if (updateFromIndex + elementCount > bufferElementCount)
        SL_THROW_FMT(EngineException, "Unable to update index data: new data out of bounds");

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferHandle);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, updateFromIndex * elementSize, elementCount * elementSize, data);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}