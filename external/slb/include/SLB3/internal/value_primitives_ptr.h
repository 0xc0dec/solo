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
#ifndef __SLB3_VALUE_PRIMITIVES_PTR__
#define __SLB3_VALUE_PRIMITIVES_PTR__

namespace SLB3 {
namespace internal {

  void  ValuePrimitivePtrPushMetatable(lua_State *L, const char *T_name);
  void* ValuePrimitivePtrGetVoidPtr(lua_State *L, const char *T_name, int index);

  template<class T>
  inline void ValuePrimitivePtrPush(lua_State *L, const char *T_name, T value) {
    T* lv = (T*) lua_newuserdata(L, sizeof(T));
    *lv = value;
    ValuePrimitivePtrPushMetatable(L, T_name);
    lua_setmetatable(L,-2);
  }
  template<class T>
  inline T ValuePrimitivePtrGet(lua_State *L, const char *T_name, int index) {
    void *ptr = ValuePrimitivePtrGetVoidPtr(L, T_name, index);
    return reinterpret_cast<T>(ptr);
  }
}

#define EXPAND_NO_CONST(Type, Name) \
  template<> \
  struct Value<Type *> { \
    typedef Type* GetType; \
    static void Push(lua_State *L, Type *v) { \
      internal::ValuePrimitivePtrPush<Type*>(L, "SLB3_ptr_"Name, v); \
    } \
    static GetType Get(lua_State *L, int index) { \
      return internal::ValuePrimitivePtrGet<Type*>(L, "SLB3_ptr_"Name, index);\
    }\
  };

#define EXPAND_CONST(Type, Name) \
  template<> \
  struct Value<const Type *> { \
    typedef const Type* GetType; \
    static void Push(lua_State *L, const Type *v) { \
      internal::ValuePrimitivePtrPush<const Type*>(L, "SLB3_const_ptr_"Name, v); \
    } \
    static GetType Get(lua_State *L, int index) { \
      return internal::ValuePrimitivePtrGet<const Type*>(L, "SLB3_const_ptr_"Name, index);\
    }\
  };

#define EXPAND(Type, Name) EXPAND_CONST(Type, Name) EXPAND_NO_CONST(Type, Name)

EXPAND(void,   "void")
EXPAND(float,  "float")
EXPAND(double, "double")
EXPAND(bool,   "bool")
EXPAND(long,   "long")
EXPAND(int,    "int")
EXPAND(short,  "short")
EXPAND(unsigned long,  "ulong")
EXPAND(unsigned int,   "uint")
EXPAND(unsigned short, "ushort")
EXPAND_NO_CONST(char, "char")

#undef EXPAND
#undef EXPAND_CONST
#undef EXPAND_NO_CONST

} // SLB3 Namespace

#endif
