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

template<T_ARGS>
struct Function<void(*)(F_ARGS)> {
  typedef void (*FType)(F_ARGS);
  static int Call(lua_State *L) {
    FType *f = (FType*) lua_touserdata(L, lua_upvalueindex(1));
    (*f)(GATHER_VALUES);
    return 0;
  }
};

template<class R COMMA T_ARGS>
struct Function<R(*)(F_ARGS)> {
  typedef R (*FType)(F_ARGS);
  static int Call(lua_State *L) {
    FType *f = (FType*) lua_touserdata(L, lua_upvalueindex(1));
    Value<R>::Push(L,(*f)(GATHER_VALUES));
    return 1;
  }
};

template<T_ARGS>
struct Value<void(*)(F_ARGS)> {
  static void Push(lua_State *L,  void(*f)(F_ARGS) ) {
    internal::PushRAWFunc(L, f, Function<void(*)(F_ARGS)>::Call);
  }
};

template<class R COMMA T_ARGS>
struct Value<R(*)(F_ARGS)> {
  static void Push(lua_State *L, R(*f)(F_ARGS) ) {
    internal::PushRAWFunc(L, f, Function<R(*)(F_ARGS)>::Call);
  }
};

#if SLB3_ENABLE_INFO_GENERATION 
template<T_ARGS>
struct Info<void(*)(F_ARGS)> : public BaseInfo {
  Type type() const { return kType_function; }
  const char *name() const { return InfoCompilerName<void(*)(F_ARGS)>::Name(); }
  int num_args() const { return N_ARGS; }
  virtual const BaseInfo** args() const { return internal::InfoArray<F_ARGS>::Compute(); }
};

template<class R COMMA T_ARGS>
struct Info<R(*)(F_ARGS)> : public BaseInfo {
  Type type() const { return kType_function; }
  const char *name() const { return InfoCompilerName<R(*)(F_ARGS)>::Name(); }
  int num_args() const { return N_ARGS; }
  const BaseInfo* type_returned() const { static Info<R> r; return &r; }
  virtual const BaseInfo** args() const { return internal::InfoArray<F_ARGS>::Compute(); }
};
#endif
