/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#include "../../../include/Solo.h"

using namespace solo;


class Demo final
{
public:
    explicit Demo(Device* device):
        device(device),
        scene(device->getScene())
    {
        initCamera();
    }

private:
    void initCamera()
    {
        auto node = scene->createNode();
        auto t = node->findComponent<Transform>();
        t->setLocalPosition(Vector3(10, 10, 10));
        t->lookAt(Vector3::zero(), Vector3::unitY());

        auto cam = node->addComponent<Camera>();
        cam->setClearColor(0.0f, 0.6f, 0.6f, 1.0f);
        cam->setNear(0.05f);

        node->addComponent<Spectator>();
    }

    Device *device = nullptr;
    Scene *scene = nullptr;
};


int main()
{
    auto device = Device::create(DeviceSetup().withMode(DeviceMode::Vulkan).withDimensions(1200, 600).withLogFilePath("demo5.log").withWindowTitle("Demo 5"));
    Demo demo(device.get());
    while (!device->isQuitRequested() && !device->isWindowCloseRequested() && !device->isKeyPressed(KeyCode::Escape, true))
    {
        device->update([&]
        {
            device->getAssetLoader()->update();
            device->getRenderer()->renderFrame([&]
            {
            });
        });
    }
    return 0;
}
