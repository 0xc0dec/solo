/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include <btBulletDynamicsCommon.h>

#define SL_TOBTVEC3(v3) btVector3(v3.x(), v3.y(), v3.z())
#define SL_FROMBTVEC3(v3) Vector3(v3.x(), v3.y(), v3.z())

#define SL_TOBTQTRN(q) btQuaternion(q.x(), q.y(), q.z(), q.w())
#define SL_FROMBTQTRN(q) Quaternion(q.w(), q.x(), q.y(), q.z())