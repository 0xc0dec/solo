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

        auto getMouseSensitivity() const -> float;
        void setMouseSensitivity(float sensitivity);

        auto getRotationAcceleration() const -> float;
        void setRotationAcceleration(float acceleration);

    private:
        Device *device = nullptr;
        Transform *transform = nullptr;

        float mouseSensitivity = 0.08f;
        float rotationAcceleration = 20;
        float movementSpeed = 10;
    };

    inline auto Spectator::getMouseSensitivity() const -> float
    {
        return mouseSensitivity;
    }

    inline void Spectator::setMouseSensitivity(float sensitivity)
    {
        mouseSensitivity = sensitivity;
    }

    inline auto Spectator::getMovementSpeed() const -> float
    {
        return movementSpeed;
    }

    inline void Spectator::setMovementSpeed(float speed)
    {
        movementSpeed = speed;
    }

    inline auto Spectator::getRotationAcceleration() const -> float
    {
        return rotationAcceleration;
    }

    inline void Spectator::setRotationAcceleration(float acceleration)
    {
        rotationAcceleration = acceleration; 
    }
}