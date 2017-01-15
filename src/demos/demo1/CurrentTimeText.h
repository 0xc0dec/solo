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
#include <chrono>
#include <iomanip>

using namespace solo;


class CurrentTimeText final: public ComponentBase<CurrentTimeText>
{
public:
    explicit CurrentTimeText(const Node &node):
        ComponentBase<CurrentTimeText>(node),
        device(node.getScene()->getDevice())
    {
    }

    void init() override final
    {
        const int textureWidth = 1024;
        const int textureHeight = 1024;
        const int lineHeight = 60;

        auto fontData = device->getFileSystem()->readBytes("../assets/aller.ttf");
        auto font = Font::create(device, fontData.data(), lineHeight, textureWidth, textureHeight, ' ', '~' - ' ', 2, 2);

        renderer = node.addComponent<FontRenderer>();
        renderer->setFont(font);
    }

    void update() override final
    {
        auto nowTimePoint = std::chrono::system_clock::now();
        auto now = std::chrono::system_clock::to_time_t(nowTimePoint);
        std::stringstream ss;
        ss << "Now: " << std::put_time(std::localtime(&now), "%X");
        renderer->setText(ss.str());
    }

private:
    Device *device = nullptr;
    FontRenderer *renderer = nullptr;
};
