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

#include "SoloSpectator.h"
#include "SoloTransform.h"
#include "SoloDevice.h"
#include "SoloRadian.h"

using namespace solo;


Spectator::Spectator(const Node &node):
    ComponentBase(node),
    device(node.getScene()->getDevice())
{
}


void Spectator::init()
{
    transform = node.findComponent<Transform>();
}


void Spectator::update()
{
    auto mouseMotion = device->getMouseMotion();
    auto dt = device->getTimeDelta();

    if (device->isMouseButtonDown(MouseButton::Right, true))
        device->setCursorCaptured(true);
    if (device->isMouseButtonReleased(MouseButton::Right))
        device->setCursorCaptured(false);

    if (device->isMouseButtonDown(MouseButton::Right, false))
    {
        if (mouseMotion.x != 0)
            transform->rotate(Vector3::unitY(), Radian(vRotSpeed * dt * -mouseMotion.x), TransformSpace::World);
        if (mouseMotion.y != 0)
        {
            auto angleToUp = Vector3::angle(transform->getLocalForward(), Vector3::unitY()).toRawRadian();
            auto delta = hRotSpeed * dt * -mouseMotion.y;
            if (delta > 0)
            {
                if (angleToUp - delta <= 0.1f)
                    delta = angleToUp - 0.1f;
            }
            else
            {
                if (angleToUp - delta >= 3.04f)
                    delta = angleToUp - 3.04f;
            }

            transform->rotate(Vector3::unitX(), Radian(delta), TransformSpace::Self);
        }
    }

    auto movement = Vector3::zero();
    if (device->isKeyPressed(KeyCode::W, false))
        movement += transform->getLocalForward();
    if (device->isKeyPressed(KeyCode::S, false))
        movement += transform->getLocalBack();
    if (device->isKeyPressed(KeyCode::A, false))
        movement += transform->getLocalLeft();
    if (device->isKeyPressed(KeyCode::D, false))
        movement += transform->getLocalRight();
    if (device->isKeyPressed(KeyCode::Q, false))
        movement += transform->getLocalDown();
    if (device->isKeyPressed(KeyCode::E, false))
        movement += transform->getLocalUp();
    movement.normalize();
    movement *= dt * movementSpeed;

    transform->translateLocal(movement);
}
