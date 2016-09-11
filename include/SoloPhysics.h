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

#include "SoloCommon.h"
#include "SoloVector3.h"
#include <vector>


namespace solo
{
    class Device;
    class DeviceToken;
    class RigidBody;

    struct RaycastResult
    {
        RigidBody* body;
        Vector3 point;
        Vector3 normal;

        RaycastResult(RigidBody* body = nullptr, const Vector3& point = Vector3::zero(), const Vector3& normal = Vector3::zero()):
            body(body), point(point), normal(normal)
        {
        }
    };

    class Physics
    {
    public:
        static auto create(Device* device, const DeviceToken& deviceToken) -> sptr<Physics>;

        SL_DISABLE_COPY_AND_MOVE(Physics)
        virtual ~Physics() {}

        virtual void update() = 0;

        virtual void setGravity(const Vector3& gravity) = 0;

        virtual auto castRay(const Vector3& from, const Vector3& to) -> RaycastResult = 0;
        virtual auto castRayAll(const Vector3& from, const Vector3& to) -> std::vector<RaycastResult> = 0;

    protected:
        Physics(Device* device, const DeviceToken&);

        Device* device;
    };
}