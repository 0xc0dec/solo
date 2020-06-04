#include <Solo.h>

using namespace solo;

auto createSkybox(Device *device, Scene *scene) -> MeshRenderer*
{
    auto layout = VertexBufferLayout();
    layout.addAttribute(VertexAttributeUsage::Position);
    layout.addAttribute(VertexAttributeUsage::Normal);
    layout.addAttribute(VertexAttributeUsage::TexCoord);
    const auto mesh = Mesh::fromFile(device, "../../../assets/meshes/quad.dae", layout);

    // TODO move lua effect files elsewhere?
    const auto effect = Effect::fromDescriptionFile(device, "../../../assets/effects/skybox.lua");
    const auto material = Material::fromEffect(device, effect);
    material->setDepthTest(true);
    material->setDepthWrite(false);
    material->setFaceCull(FaceCull::None);
    material->bindParameter("matrices:proj", ParameterBinding::ProjectionMatrix);
    material->bindParameter("matrices:worldView", ParameterBinding::WorldViewMatrix);

    const auto skyboxNode = scene->createNode();
    const auto renderer = skyboxNode->addComponent<MeshRenderer>();
    renderer->setMesh(mesh);
    renderer->setMaterial(0, material);

    const auto texture = CubeTexture::fromFaceFiles(device,
        "../../../assets/textures/skyboxes/deep-space/+x.png",
        "../../../assets/textures/skyboxes/deep-space/-x.png",
        "../../../assets/textures/skyboxes/deep-space/+y.png",
        "../../../assets/textures/skyboxes/deep-space/-y.png",
        "../../../assets/textures/skyboxes/deep-space/+z.png",
        "../../../assets/textures/skyboxes/deep-space/-z.png"
    );
    texture->setWrap(TextureWrap::ClampToEdge);

    material->setTextureParameter("colorMap", texture);

    return renderer;
}