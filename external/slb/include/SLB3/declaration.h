// Copyright (c) 2013, Jose L. Hidalgo Valiño <pplux@pplux.com>
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
#ifndef __SLB3_DECLARATION__
#define __SLB3_DECLARATION__ 1

#include "info.h"
#include "internal/class.h"
#include "policy.h"

 #define SLB3_DECLARATION(Type, Name, PolicyName) \
  namespace SLB3 { \
    template<>\
    struct Wrapper<Type> { \
      static const char *GetName() { return Name; } \
      typedef PolicyName::Implementation<Type> Policy; \
      static void Register(internal::Class<Type> &ref); \
    };\
  }\
  SLB3_INTERNAL_INFO_DECLARATION(Type, Name)

  #define SLB3_ENUM_DECLARATION(Type) \
  namespace SLB3 {\
    template<> \
    struct Value<Type> { \
      typedef Type GetType; \
      static void Push(lua_State *L, const Type v) { \
        lua_Integer *data = (lua_Integer*) lua_newuserdata(L, sizeof(lua_Integer)); \
        *data = v; \
        internal::PushEnumMetatable(L, #Type); \
        lua_setmetatable(L,-2); \
      } \
      static GetType Get(lua_State *L, int p) { \
        lua_Integer *data = (lua_Integer*) luaL_checkudata(L, p, #Type); \
        Type result = (Type) *data;\
        return result; \
      } \
    }; \
    template<> \
    struct Value<const Type> : public Value<Type> {}; \
  }

  #define SLB3_POD_DECLARATION(Type) \
  void SLB3_POD_Get(lua_State*, int, Type*); \
  namespace SLB3 {\
    template<> \
    struct Value<const Type&> { \
      typedef Type GetType; \
      static GetType Get(lua_State *L, int p) { \
        GetType t; \
        SLB3_POD_Get(L, p, &t); \
        return t; \
      } \
    }; \
    template<> \
    struct Value<const Type> : public Value<const Type&> {}; \
    template<> \
    struct Value<const Type*> : public Value<const Type&> {}; \
    template<> \
    struct Value<Type*> : public Value<const Type&> {}; \
  }

#endif

