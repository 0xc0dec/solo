#pragma once

#include "../../include/Solo.h"


class Screenshoter: public solo::ComponentBase<Screenshoter>
{
public:
    explicit Screenshoter(const solo::Node& node, const std::string& path):
        ComponentBase<Screenshoter>(node),
        device(node.getScene()->getDevice()),
        path(path)
    {
    }

    virtual void update() override final
    {
        if (device->isKeyPressed(solo::KeyCode::P, true))
            device->saveScreenshot(path);
    }

private:
    solo::Device* device;
    std::string path;
};