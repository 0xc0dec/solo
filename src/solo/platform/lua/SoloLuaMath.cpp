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

#include "SoloRadian.h"
#include "SoloVector2.h"
#include "SoloLuaCommon.h"

using namespace solo;
using namespace LuaIntf;


void registerRadian(CppBindModule<LuaBinding> &module)
{
    auto radian = module.beginClass<Radian>("Radian");
    radian.endClass();
}


void registerVector2(CppBindModule<LuaBinding> &module)
{
    auto vector2 = module.beginClass<Vector2>("Vector2");
    vector2.addConstructor(LUA_ARGS(float, float));
    
    REGISTER_VARIABLE(vector2, Vector2, x);
    REGISTER_VARIABLE(vector2, Vector2, y);
    
    REGISTER_STATIC_METHOD(vector2, Vector2, zero);
    REGISTER_STATIC_METHOD(vector2, Vector2, unit);
    REGISTER_STATIC_METHOD(vector2, Vector2, unitX);
    REGISTER_STATIC_METHOD(vector2, Vector2, unitY);
    
    REGISTER_METHOD(vector2, Vector2, isUnit);
    REGISTER_METHOD(vector2, Vector2, isZero);

    REGISTER_STATIC_METHOD(vector2, Vector2, angle);
    REGISTER_METHOD(vector2, Vector2, clamp);

    REGISTER_METHOD(vector2, Vector2, distance);
    REGISTER_METHOD(vector2, Vector2, distanceSquared);

    REGISTER_STATIC_OVERLOADED_METHOD(vector2, Vector2, dot, "dot", float, , const Vector2&, const Vector2&);
    REGISTER_OVERLOADED_METHOD(vector2, Vector2, dot, "dot", float, const, const Vector2&);
    
    REGISTER_METHOD(vector2, Vector2, length);
    REGISTER_METHOD(vector2, Vector2, lengthSquared);
    
    REGISTER_METHOD(vector2, Vector2, normalize);
    REGISTER_METHOD(vector2, Vector2, normalized);

    vector2.endClass();
}