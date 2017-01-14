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

#pragma once

#include "../../../include/Solo.h"

using namespace solo;


class DemoBase
{
public:
    explicit DemoBase(Device *device):
        device(device),
        scene(device->getScene()),
        loader(device->getAssetLoader()),
        canvasSize(device->getCanvasSize())
    {
    }

    virtual ~DemoBase() {}

    void run()
    {
        while (!device->isQuitRequested() && !device->isWindowCloseRequested() && !device->isKeyPressed(KeyCode::Escape, true))
        {
            device->update([&]
            {
                device->getAssetLoader()->update();
                device->getPhysics()->update();
                device->getRenderer()->renderFrame([&]
                {
                    update();
                    render();
                });
            });
        }
    }

protected:
    Device *device = nullptr;
    Scene *scene = nullptr;
    AssetLoader *loader = nullptr;
    Vector2 canvasSize;

    virtual void render() = 0;

    virtual void update()
    {
        scene->visit([](Component *cmp) { cmp->update(); });
    }

    void renderByTags(uint32_t tags, const RenderContext &ctx)
    {
        scene->visit(tags, [=](Component *cmp) { cmp->render(ctx); });
    }
};
