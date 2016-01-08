#include "SoloDevice.h"
#include "SoloIndexedMeshPart.h"
#include "platform/stub/SoloStubMesh.h"
#include "platform/opengl/SoloOpenGLMesh.h"

using namespace solo;


shared<Mesh> Mesh::create(DeviceMode mode)
{
    if (mode == DeviceMode::OpenGL)
        return SL_NEW_SHARED(OpenGLMesh);
    return SL_NEW_SHARED(StubMesh);
}


void Mesh::rebuildAsQuad()
{
    float data[] = {
        -1, -1, 0,  0, 0, -1,   0, 0,
        -1, 1, 0,   0, 0, -1,   0, 1,
        1, 1, 0,    0, 0, -1,   1, 1,
        1, -1, 0,   0, 0, -1,   1, 0
    };
    unsigned short indices[] = { 0, 1, 2, 0, 2, 3 };

    VertexFormat vf({
        VertexFormatElement(VertexFormatElementSemantics::Position, 3),
        VertexFormatElement(VertexFormatElementSemantics::Normal, 3),
        VertexFormatElement(VertexFormatElementSemantics::TexCoord0, 2)
    });
    resetVertexData(vf, data, 4, false);
    setPrimitiveType(MeshPrimitiveType::Triangles);
    
    auto part = addIndexedPart();
    part->resetIndexData(MeshIndexFormat::UnsignedShort, indices, 6, false);
    part->setPrimitiveType(MeshPrimitiveType::Triangles);
}


void Mesh::rebuildAsBox()
{
    float data[] = {
        -1, -1, 1,  0, 0,
        -1, 1, 1,   0, 1,
        1, 1, 1,    1, 1,
        1, -1, 1,   1, 0,

        -1, -1, -1, 0, 0,
        -1, 1, -1,  0, 1,
        -1, 1, 1,   1, 1,
        -1, -1, 1,  1, 0,

        1, -1, -1,  0, 0,
        1, 1, -1,   0, 1,
        -1, 1, -1,  1, 1,
        -1, -1, -1, 1, 0,

        1, -1, 1,   0, 0,
        1, 1, 1,    0, 1,
        1, 1, -1,   1, 1,
        1, -1, -1,  1, 0,

        -1, 1, 1,   0, 0,
        -1, 1, -1,  0, 1,
        1, 1, -1,   1, 1,
        1, 1, 1,    1, 0,

        -1, -1, -1, 0, 0,
        -1, -1, 1,  0, 1,
        1, -1, 1,   1, 1,
        1, -1, -1,  1, 0

    };
    unsigned short indices[] = {
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

    VertexFormat vf({
        VertexFormatElement(VertexFormatElementSemantics::Position, 3),
        VertexFormatElement(VertexFormatElementSemantics::TexCoord0, 2)
    });

    resetVertexData(vf, data, 24, false);
    setPrimitiveType(MeshPrimitiveType::Triangles);

    auto part = addIndexedPart();
    part->resetIndexData(MeshIndexFormat::UnsignedShort, indices, 36, false);
    part->setPrimitiveType(MeshPrimitiveType::Triangles);
}
