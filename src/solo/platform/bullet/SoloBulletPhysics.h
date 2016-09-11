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

#include "SoloPhysics.h"
#include <btBulletDynamicsCommon.h>


namespace solo
{
    class BulletPhysics final : public Physics
    {
    public:
        BulletPhysics(Device* device, const DeviceToken& deviceToken);

        void update() override final;

        void setGravity(const Vector3& gravity) override final;

        auto castRay(const Vector3& from, const Vector3& to) -> RaycastResult override;
        auto castRayAll(const Vector3& from, const Vector3& to) -> std::vector<RaycastResult> override;

        auto getWorld() const -> btDiscreteDynamicsWorld*;

    private:
        // Note: order matters for the proper destruction
        uptr<btBroadphaseInterface> broadPhase;
        uptr<btCollisionConfiguration> collisionConfig;
        uptr<btCollisionDispatcher> collisionDispatcher;
        uptr<btSequentialImpulseConstraintSolver> solver;
        uptr<btDiscreteDynamicsWorld> world;
    };
}