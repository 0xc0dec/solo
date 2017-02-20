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

#include <btBulletDynamicsCommon.h>

#define SL_TOBTVEC3(v3) btVector3(v3.x, v3.y, v3.z)
#define SL_FROMBTVEC3(v3) Vector3(v3.x(), v3.y(), v3.z())

#define SL_TOBTQTRN(q) btQuaternion(q.x, q.y, q.z, q.w)
#define SL_FROMBTQTRN(q) Quaternion(q.x(), q.y(), q.z(), q.w())