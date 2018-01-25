/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloSpectator.h"
#include "SoloTransform.h"
#include "SoloDevice.h"
#include "SoloRadians.h"

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
    const auto mouseMotion = device->getMouseMotion();
    const auto dt = device->getTimeDelta();

    if (device->isMouseButtonDown(MouseButton::Right, true))
        device->setCursorCaptured(true);
    if (device->isMouseButtonReleased(MouseButton::Right))
        device->setCursorCaptured(false);

    if (device->isMouseButtonDown(MouseButton::Right, false))
    {
        if (mouseMotion.x != 0)
            transform->rotateByAxisAngle({0, 1, 0}, Radians(mouseSensitivity * -mouseMotion.x), TransformSpace::World);

        if (mouseMotion.y != 0)
        {
            const auto angleToUp = transform->getLocalForward().angle({0, 1, 0}).toRawRadians();
            auto delta = mouseSensitivity * -mouseMotion.y;
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

            transform->rotateByAxisAngle({1, 0, 0}, Radians(delta), TransformSpace::Self);
        }
    }

    auto movement = Vector3();
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
