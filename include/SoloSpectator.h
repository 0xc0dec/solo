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

#include "SoloComponent.h"


namespace solo
{
    class Transform;
    class Device;

    class Spectator final: public ComponentBase<Spectator>
    {
    public:
        explicit Spectator(const Node &node);

        void init() override final;
        void update() override final;

        auto getMovementSpeed() const -> float;
        void setMovementSpeed(float speed);

        auto getHorizontalRotationSpeed() const -> float;
        void setHorizontalRotationSpeed(float speed);

        auto getVerticalRotationSpeed() const -> float;
        void setVerticalRotationSpeed(float speed);

    private:
        Device *device = nullptr;
        Transform *transform = nullptr;

        float hRotSpeed = 0.5f;
        float vRotSpeed = 0.5f;
        float movementSpeed = 10;
    };

    inline auto Spectator::getHorizontalRotationSpeed() const -> float
    {
        return hRotSpeed;
    }

    inline void Spectator::setHorizontalRotationSpeed(float speed)
    {
        hRotSpeed = speed;
    }

    inline auto Spectator::getVerticalRotationSpeed() const -> float
    {
        return vRotSpeed;
    }

    inline void Spectator::setVerticalRotationSpeed(float speed)
    {
        vRotSpeed = speed;
    }

    inline auto Spectator::getMovementSpeed() const -> float
    {
        return movementSpeed;
    }

    inline void Spectator::setMovementSpeed(float speed)
    {
        movementSpeed = speed;
    }
}