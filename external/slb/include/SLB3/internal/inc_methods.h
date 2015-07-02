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

// This file is ment to be included with the following defines:
// N_ARGS = Number of arguments
// T_ARGS = class A1, class A2
// F_ARGS = A1,A2
// GATHER_VALUES = Value<A1>::Get(L,2), Value<A2>::Get(L,3)
// COMMA , //(if args >0)

#define JOIN2(A,B) A##B
#define JOIN(A,B) JOIN2(A,B)

namespace internal {
  template<class FType, class C, class R COMMA T_ARGS >
  struct JOIN(MethodImp,N_ARGS){
    static R Call(lua_State *L) {
      FType *f = (FType*) lua_touserdata(L, lua_upvalueindex(1));
      C obj = Value<C>::Get(L,1);
      if (!obj) luaL_error(L,
          "Expected non-null object(%s) as first argument to method call",
          Wrapper<typename Value<C>::ClassType>::GetName());
      // remove object from first pos
      lua_remove(L,1);
      return (obj->**f)(GATHER_VALUES);
    }
  };
}

/* Non-const Version, Returns void */
template<class C COMMA T_ARGS>
struct Function<void(C::*)(F_ARGS)> {
  static int Call(lua_State *L) {
    internal::JOIN(MethodImp,N_ARGS)<void(C::*)(F_ARGS), C*, void COMMA F_ARGS>::Call(L); \
    return 0;
  }
};

/* Const version, returns void */
template<class C COMMA T_ARGS>
struct Function<void(C::*)(F_ARGS) const> {
  static int Call(lua_State *L) {
    internal::JOIN(MethodImp,N_ARGS)<void(C::*)(F_ARGS) const, const C*, void COMMA F_ARGS>::Call(L); \
    return 0;
  }
};

/* Non-const version, returns something */
template<class C, class R COMMA T_ARGS>
struct Function<R(C::*)(F_ARGS)> {
  static int Call(lua_State *L) {
    Value<R>::Push(L,
        internal::JOIN(MethodImp,N_ARGS)<R(C::*)(F_ARGS), C*, R COMMA F_ARGS>::Call(L));
    return 1;
  }
};

/* Const version, returns something */
template<class C, class R COMMA T_ARGS>
struct Function<R(C::*)(F_ARGS) const> {
  static int Call(lua_State *L) {
    Value<R>::Push(L,
        internal::JOIN(MethodImp,N_ARGS)<R(C::*)(F_ARGS) const, const C*, R COMMA F_ARGS>::Call(L));
    return 1;
  }
};

// - Value specializations -----------------------------------------------------

template<class C COMMA T_ARGS>
struct Value<void(C::*)(F_ARGS)> {
  static void Push(lua_State *L, void(C::*m)(F_ARGS) ) {
    internal::PushRAWFunc(L, m, Function<void(C::*)(F_ARGS)>::Call);
  }
};

template<class C COMMA T_ARGS>
struct Value<void(C::*)(F_ARGS) const> {
  static void Push(lua_State *L,void(C::*m)(F_ARGS) const ) {
    internal::PushRAWFunc(L, m, Function<void(C::*)(F_ARGS) const>::Call);
  }
};

template<class C, class R COMMA T_ARGS>
struct Value<R(C::*)(F_ARGS)> {
  static void Push(lua_State *L, R(C::*m)(F_ARGS)) {
    internal::PushRAWFunc(L, m, Function<R(C::*)(F_ARGS)>::Call);
  }
};

template<class C, class R COMMA T_ARGS>
struct Value<R(C::*)(F_ARGS) const> {
  static void Push(lua_State *L,R(C::*m)(F_ARGS) const) {
    internal::PushRAWFunc(L, m, Function<R(C::*)(F_ARGS) const>::Call);
  }
};


#if SLB3_ENABLE_INFO_GENERATION 
template<class C COMMA T_ARGS>
struct Info<void(C::*)(F_ARGS)> : public BaseInfo {
  Type type() const { return kType_function; }
  const char *name() const { return InfoCompilerName<void(C::*)(F_ARGS)>::Name(); }
  int num_args() const { return N_ARGS+1; }
  virtual const BaseInfo** args() const { return internal::InfoArray<C* COMMA F_ARGS>::Compute(); }
};

template<class C, class R COMMA T_ARGS>
struct Info<R(C::*)(F_ARGS)> : public BaseInfo {
  Type type() const { return kType_function; }
  const char *name() const { return InfoCompilerName<R(C::*)(F_ARGS)>::Name(); }
  int num_args() const { return N_ARGS+1; }
  const BaseInfo* type_returned() const { static Info<R> r; return &r; }
  virtual const BaseInfo** args() const { return internal::InfoArray<C* COMMA F_ARGS>::Compute(); }
};

template<class C COMMA T_ARGS>
struct Info<void(C::*)(F_ARGS) const> : public BaseInfo {
  Type type() const { return kType_function; }
  const char *name() const { return InfoCompilerName<void(C::*)(F_ARGS) const>::Name(); }
  int num_args() const { return N_ARGS+1; }
  virtual const BaseInfo** args() const { return internal::InfoArray<const C* COMMA F_ARGS>::Compute(); }
};

template<class C, class R COMMA T_ARGS>
struct Info<R(C::*)(F_ARGS) const> : public BaseInfo {
  Type type() const { return kType_function; }
  const char *name() const { return InfoCompilerName<R(C::*)(F_ARGS) const>::Name(); }
  int num_args() const { return N_ARGS+1; }
  const BaseInfo* type_returned() const { static Info<R> r; return &r; }
  virtual const BaseInfo** args() const { return internal::InfoArray<const C* COMMA F_ARGS>::Compute(); }
};
#endif

#undef JOIN
#undef JOIN2
