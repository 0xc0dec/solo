#include "SoloOpenGLIndexedMeshPart.h"

using namespace solo;


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