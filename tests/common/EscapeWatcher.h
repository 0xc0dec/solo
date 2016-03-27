#pragma once

#include "../../include/Solo.h"


class EscapeWatcher final: public solo::ComponentBase<EscapeWatcher>
{
public:
    explicit EscapeWatcher(const solo::Node& node):
        ComponentBase<EscapeWatcher>(node),
        device(node.getScene()->getDevice())
    {
    }

    virtual void update() override final
    {
        if (device->isKeyPressed(solo::KeyCode::Escape, true))
            device->stopRunning();
    }

private:
    solo::Device* device;
};