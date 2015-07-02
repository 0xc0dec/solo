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
#ifndef __SLB3_CLASS_IMPLEMENTATION__
#define __SLB3_CLASS_IMPLEMENTATION__ 1

#include <cassert>
#include "../value.h"

namespace SLB3 {
namespace internal {

void OnClassDefinitionBegins(lua_State *L, int meta_index);
void OnClassDefinitionEnds(lua_State *L, int meta_index);

template<class T>
inline int DestructorWrapper(lua_State *L) {
  Object obj(lua_touserdata(L,1));
  Wrapper<T>::Policy::Destructor(obj);
  return 0;
}

#if SLB3_ENABLE_INFO_GENERATION
template<class T>
struct WrappedClassInfo : public BaseInfo {
  Type type() const { return kType_class; }
  const char *name() const { return Wrapper<T>::GetName(); }
  int num_args() const { return 0; }
};
#endif

template<class T>
inline Class<T>::Class(lua_State *L, int index)
    : Namespace(L, Wrapper<T>::GetName())
    , meta_(lua_absindex(L,index)) {

  assert(lua_istable(L, meta_) && "Invalid metatable");
  int top = lua_gettop(L);
  // push ourselves as "__index" of meta
  push(L);
  lua_setfield(L_, meta_, "__index");
  lua_pushstring(L, Wrapper<T>::GetName());
  lua_setfield(L_, meta_, "__class");
  // Create __gc method
  lua_pushcclosure(L_, DestructorWrapper<T>, 0);
  lua_setfield(L_, meta_, "__gc");
  //createa a metatable for ourselves
  push(L);
  lua_newtable(L);
  lua_setmetatable(L,-2);
  lua_pop(L,1);
  // Notify new defition begins
  OnClassDefinitionBegins(L, meta_);
#if SLB3_ENABLE_INFO_GENERATION
  static WrappedClassInfo<T> info; 
  internal::RegisterInfoClass(L, meta_, &info);
#endif
  // sanity Check
  assert(top == lua_gettop(L) && "screwed stack at class creation");
}

template<class T>
inline Class<T>::~Class() {
  // Notify finish of class definition
  OnClassDefinitionEnds(L_, meta_);
}

void private_SetCallMetamethod(lua_State *L, int class_meta_
    , lua_CFunction conversion_func);

template<class T>
inline Class<T>& Class<T>::constructor() {
  private_SetCallMetamethod(
      L_, meta_, Constructor<T*(*)()>::Call);
  return *this;
}

template<class T>
template<class A1>
inline Class<T>& Class<T>::constructor() {
  private_SetCallMetamethod(
      L_, meta_, Constructor<T*(*)(A1)>::Call);
  return *this;
}

template<class T>
template<class A1, class A2>
inline Class<T>& Class<T>::constructor() {
  private_SetCallMetamethod(
      L_, meta_, Constructor<T*(*)(A1,A2)>::Call);
  return *this;
}

template<class T>
template<class A1, class A2, class A3>
inline Class<T>& Class<T>::constructor() {
  private_SetCallMetamethod(
      L_, meta_, Constructor<T*(*)(A1,A2,A3)>::Call);
  return *this;
}

template<class T>
template<class A1, class A2, class A3, class A4>
inline Class<T>& Class<T>::constructor() {
  private_SetCallMetamethod(
      L_, meta_, Constructor<T*(*)(A1,A2,A3,A4)>::Call);
  return *this;
}

template<class T>
template<class A1, class A2, class A3, class A4, class A5>
inline Class<T>& Class<T>::constructor() {
  private_SetCallMetamethod(
      L_, meta_, Constructor<T*(*)(A1,A2,A3,A4,A5)>::Call);
  return *this;
}

template<class T>
template<class R>
Class<T>& Class<T>::setMetamethod(const char *name, R v) {
  Value<R>::Push(L_, v);
  lua_setfield(L_, meta_, name);
  return *this;
}


//-- Inheritance mechanism -----------------------------------------------

template<class C, class P>
inline void* private_ConvertTo(void* c_ptr) {
  P* p_ptr = reinterpret_cast<C*>(c_ptr);
  return p_ptr;
}

// pops a value from the top of the stack (the parent's metatable) and
// setups the inheritance function;
void private_AddInheritance(
    lua_State *L,
    int class_meta_,
    UnpackFunc objects_unpack_,
    UnpackFunc parents_unpack_,
    ConversionFunc_T conversion_func);

template<class T>
template<class P>
inline Class<T>& Class<T>::inherits() {
  typedef typename Wrapper<T>::Policy P_obj;
  typedef typename Wrapper<P>::Policy P_parent;
  PushMetatable<P>(L_);
  private_AddInheritance(
      L_, meta_,
      P_obj::Unpack, P_parent::Unpack,
      private_ConvertTo<T,P>);
  return *this;
}

} // end of internal namespace
} // end of SLB3 namespace

#endif
