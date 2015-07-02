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
// N_ARGS = Number of arguments
// T_ARGS = class A1, class A2
// F_ARGS = A1,A2
// GATHER_VALUES = Value<A1>::Get(L,2), Value<A2>::Get(L,3)
// COMMA , //(if args >0)

template<class T COMMA T_ARGS>
struct Constructor<T*(*)(F_ARGS)> {
  static int Call(lua_State *L) {
    lua_remove(L,1); // remove the table from the first argument
    void *where_mem = NULL;
    Object holder = Wrapper<T>::Policy::NewInstanceBegin(L, &where_mem);
    internal::PushMetatable<T>(L);
    lua_setmetatable(L,-2);
    T* new_obj = new (where_mem) T(GATHER_VALUES);
    Wrapper<T>::Policy::NewInstanceEnd(holder, new_obj);
    return 1;
  }
};

