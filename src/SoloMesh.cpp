#include "SoloDevice.h"
#include "SoloIndexedMeshPart.h"
#include "platform/stub/SoloStubMesh.h"
#include "platform/opengl/SoloOpenGLMesh.h"

using namespace solo;


static void rebuildMesh(Mesh *mesh, float *data, unsigned elementCount, unsigned short *indexData, unsigned indexElementCount)
{
    mesh->resetStorage(0, data, elementCount, false);
    mesh->setPrimitiveType(MeshPrimitiveType::Triangles);

    auto part = mesh->getPartCount() > 0 ? mesh->getPart(0) : mesh->addPart(MeshIndexFormat::UnsignedShort);
    part->resetIndexData(indexData, indexElementCount, false);
    part->setPrimitiveType(MeshPrimitiveType::Triangles);
}


shared<Mesh> Mesh::create(DeviceMode mode, const VertexFormat &vertexFormat)
{
    if (mode == DeviceMode::OpenGL)
        return SL_NEW_SHARED(OpenGLMesh, vertexFormat);
    return SL_NEW_SHARED(StubMesh, vertexFormat);
}


shared<Mesh> Mesh::createPrefab(DeviceMode mode, MeshPrefab prefab)
{
    switch (prefab)
    {
    case MeshPrefab::Quad: return createQuadMesh(mode);
    case MeshPrefab::Cube: return createBoxMesh(mode);
    default:
        SL_THROW_FMT(EngineException, "Unknown mesh prefab");
    }
}


shared<Mesh> Mesh::createQuadMesh(DeviceMode mode)
{
    VertexFormat vf({
        VertexFormatElement(VertexFormatElementSemantics::Position, 3, 0),
        VertexFormatElement(VertexFormatElementSemantics::Normal, 3, 0),
        VertexFormatElement(VertexFormatElementSemantics::TexCoord0, 2, 0)
    });
    float data[] = {
        -1, -1, 0,  0, 0, -1,   0, 0,
        -1, 1, 0,   0, 0, -1,   0, 1,
        1, 1, 0,    0, 0, -1,   1, 1,
        1, -1, 0,   0, 0, -1,   1, 0
    };
    unsigned short indices[] = { 0, 1, 2, 0, 2, 3 };

    auto quad = create(mode, vf);
    rebuildMesh(quad.get(), data, 4, indices, 6);
    return quad;
}


shared<Mesh> Mesh::createBoxMesh(DeviceMode mode)
{
    VertexFormat vf({
        VertexFormatElement(VertexFormatElementSemantics::Position, 3, 0),
        VertexFormatElement(VertexFormatElementSemantics::TexCoord0, 2, 0)
    });
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

    auto box = create(mode, vf);
    rebuildMesh(box.get(), data, 24, indices, 36);
    return box;
}
