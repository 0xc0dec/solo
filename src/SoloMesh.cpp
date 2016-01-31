#include "SoloDevice.h"
#include "platform/stub/SoloStubMesh.h"
#include "platform/opengl/SoloOpenGLMesh.h"

using namespace solo;


shared<Mesh> Mesh::create(DeviceMode mode, const VertexFormat& vertexFormat)
{
    if (mode == DeviceMode::OpenGL)
        return SL_NEW_SHARED(OpenGLMesh, vertexFormat);
    return SL_NEW_SHARED(StubMesh, vertexFormat);
}


shared<Mesh> Mesh::createPrefab(DeviceMode mode, MeshPrefab prefab)
{
    switch (prefab)
    {
    case MeshPrefab::Quad:
        return createQuadMesh(mode);
    case MeshPrefab::Cube:
        return createBoxMesh(mode);
    default:
        SL_THROW_FMT(EngineException, "Unknown mesh prefab");
    }
}


shared<Mesh> Mesh::createQuadMesh(DeviceMode mode)
{
    float data[] =
    {
        -1, -1, 0,  0, 0, -1,   0, 0,
        -1, 1, 0,   0, 0, -1,   0, 1,
        1, 1, 0,    0, 0, -1,   1, 1,
        1, -1, 0,   0, 0, -1,   1, 0
    };
    unsigned short indices[] = { 0, 1, 2, 0, 2, 3 };
    VertexFormat vf(
    {
        VertexFormatElement(VertexFormatElementSemantics::Position, 3, 0),
        VertexFormatElement(VertexFormatElementSemantics::Normal, 3, 0),
        VertexFormatElement(VertexFormatElementSemantics::TexCoord0, 2, 0)
    });

    auto mesh = create(mode, vf);

    mesh->resetData(0, data, 4, false);

    auto index = mesh->getIndexCount() > 0 ? 0 : mesh->addIndex(MeshIndexFormat::UnsignedShort);
    mesh->resetIndexData(index, indices, 6, false);
    mesh->setIndexPrimitiveType(index, MeshPrimitiveType::Triangles);

    return mesh;
}


shared<Mesh> Mesh::createBoxMesh(DeviceMode mode)
{
    float vertexData[] =
    {
        -1, -1, 1,      -1, 1, 1,       1, 1, 1,        1, -1, 1,
        -1, -1, -1,     -1, 1, -1,      -1, 1, 1,       -1, -1, 1,
        1, -1, -1,      1, 1, -1,       -1, 1, -1,      -1, -1, -1,
        1, -1, 1,       1, 1, 1,        1, 1, -1,       1, -1, -1,
        -1, 1, 1,       -1, 1, -1,      1, 1, -1,       1, 1, 1,

        -1, -1, -1,     -1, -1, 1,      1, -1, 1,       1, -1, -1,
    };
    float texCoordData[] =
    {
        0, 0,   0, 1,   1, 1,   1, 0,
        0, 0,   0, 1,   1, 1,   1, 0,
        0, 0,   0, 1,   1, 1,   1, 0,
        0, 0,   0, 1,   1, 1,   1, 0,
        0, 0,   0, 1,   1, 1,   1, 0,
        0, 0,   0, 1,   1, 1,   1, 0
    };
    unsigned short indices[] =
    {
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
    VertexFormat vf(
    {
        VertexFormatElement(VertexFormatElementSemantics::Position, 3, 0),
        VertexFormatElement(VertexFormatElementSemantics::TexCoord0, 2, 1)
    });

    auto mesh = create(mode, vf);

    mesh->resetData(0, vertexData, 24, false);
    mesh->resetData(1, texCoordData, 24, false);

    auto index = mesh->getIndexCount() > 0 ? 0 : mesh->addIndex(MeshIndexFormat::UnsignedShort);
    mesh->resetIndexData(index, indices, 36, false);
    mesh->setIndexPrimitiveType(index, MeshPrimitiveType::Triangles);

    return mesh;
}
