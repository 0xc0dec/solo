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

#include "SoloRigidBody.h"
#include <btBulletDynamicsCommon.h>

namespace solo
{
    class Transform;
    class Collider;
    
    namespace bullet
    {
        class Collider;

        class RigidBody final : public solo::RigidBody
        {
        public:
            RigidBody(const Node &node, const RigidBodyConstructionParameters &parameters);
            virtual ~RigidBody();

            void setCollider(sptr<solo::Collider> collider) override final;

            void onTransformChanged(const Transform *transform, uint32_t dirtyFlags) override final;

        private:
            float mass = 0;
            sptr<solo::Collider> collider;
            btCollisionShape *shape;
            Transform *transformCmp;
            btDiscreteDynamicsWorld *world;
            uptr<btMotionState> motionState;
            uptr<btRigidBody> body;

            void syncScale();
        };
    }
}