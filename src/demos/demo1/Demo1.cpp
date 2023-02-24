/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include <Solo.h>

using namespace solo;

auto createSkybox(Device *device, Scene *scene) -> MeshRenderer*;

auto createMainCamera(Scene *scene) -> Camera * {
    const auto node = scene->createNode();
    node->addComponent<Spectator>();
    const auto camera = node->addComponent<Camera>();
    camera->setClearColor({0, 0.6, 0.6, 1.0});
    camera->setZNear(0.05);
    return camera;
}

int main() {
    const auto device = Device::create({DeviceMode::OpenGL, 1366, 768, false, false, "Demo 1"});
    const auto scene = Scene::empty(device.get());

    createSkybox(device.get(), scene.get());
    const auto camera = createMainCamera(scene.get());

    while (true) {
        device->update([&]() {
            scene->update();
            camera->renderFrame([&]() {
                scene->render(~0);
            });
        });

        if (device->isQuitRequested() ||
                device->isKeyPressed(KeyCode::Escape, true) ||
                device->isWindowCloseRequested())
            break;
    }

    return 0;
}
