/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include <Solo.h>

using namespace solo;

auto createSkybox(Device *device, Scene *scene) -> MeshRenderer*
{
    auto layout = VertexBufferLayout();
    layout.addSemanticAttribute(VertexAttributeSemantics::Position);
    layout.addSemanticAttribute(VertexAttributeSemantics::Normal);
    layout.addSemanticAttribute(VertexAttributeSemantics::TexCoord);
    const auto mesh = Mesh::loadFromFile(device, "../../assets/meshes/quad.dae", layout);

    const auto effect = Effect::loadFromDescriptionFile(device, "../../src/demos/lua/effects/skybox.lua");
    const auto material = Material::create(device, effect);
    material->setDepthTest(true);
    material->setDepthWrite(false);
    material->setFaceCull(FaceCull::None);
    material->bindParameter("matrices:proj", BindParameterSemantics::ProjectionMatrix);
    material->bindParameter("matrices:worldView", BindParameterSemantics::WorldViewMatrix);

    const auto skyboxNode = scene->createNode();
    const auto renderer = skyboxNode->addComponent<MeshRenderer>();
    renderer->setMesh(mesh);
    renderer->setMaterial(0, material);

    const auto texture = CubeTexture::loadFromFaceFiles(device,
        "../../assets/textures/skyboxes/deep-space/+x.png",
        "../../assets/textures/skyboxes/deep-space/-x.png",
        "../../assets/textures/skyboxes/deep-space/+y.png",
        "../../assets/textures/skyboxes/deep-space/-y.png",
        "../../assets/textures/skyboxes/deep-space/+z.png",
        "../../assets/textures/skyboxes/deep-space/-z.png"
    );
    texture->setWrap(TextureWrap::ClampToEdge);

    material->setTextureParameter("colorMap", texture);

    return renderer;
}

int main(int argc, s8 *argv[])
{
    const auto device = Device::create({DeviceMode::OpenGL, 1366, 768, false, false, "Demo 1"});
    const auto scene = Scene::create(device.get());

    const auto skyboxRenderer = createSkybox(device.get(), scene.get());

    const auto cameraNode = scene->createNode();
    
    cameraNode->addComponent<Spectator>();

    const auto camera = cameraNode->addComponent<Camera>();
    camera->setClearColor({0, 0.6, 0.6, 1.0});
    camera->setZNear(0.05);

    while (true)
    {
        device->update([&]()
        {
            scene->visit([&](auto cmp) { cmp->update(); });
        });

        camera->renderFrame([&]()
        {
            skyboxRenderer->render();
        });

        if (device->isQuitRequested() ||
            device->isKeyPressed(KeyCode::Escape, true) ||
            device->isWindowCloseRequested())
            break;
    }

    return 0;
}
