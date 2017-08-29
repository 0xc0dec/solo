/*
    Copyright (c) Aleksey Fedotov
    MIT license
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
            transform->rotateByAxisAngle(Vector3::unitY(), Radian(mouseSensitivity * dt * -mouseMotion.x), TransformSpace::World);

        if (mouseMotion.y != 0)
        {
            auto angleToUp = Vector3::angle(transform->getLocalForward(), Vector3::unitY()).toRawRadian();
            auto delta = mouseSensitivity * dt * -mouseMotion.y;
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

            transform->rotateByAxisAngle(Vector3::unitX(), Radian(delta), TransformSpace::Self);
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
