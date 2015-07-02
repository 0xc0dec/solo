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
#ifndef __SLB3_FUNCTIONS__
#define __SLB3_FUNCTIONS__ 1

namespace SLB3 {

template<class>
struct Function {
};

template<class>
struct Constructor {
};

namespace internal {
  template<class T>
  inline void PushRAWFunc(lua_State *L, T f, lua_CFunction lua_call) {
    T *copy = (T*)lua_newuserdata(L, sizeof(T));
    *copy = f;
    int num_upvalues = 1;
#if SLB3_ENABLE_INFO_GENERATION
    static Info<T> func_info;
    internal::PushInfoUpvalue(L, &func_info);
    num_upvalues = 2;
#endif
    lua_pushcclosure(L, lua_call, num_upvalues);
  }
}

/* Expand for 0 arguments */
#define N_ARGS 0
#define T_ARGS /*Nothing*/
#define F_ARGS /*Nothing*/
#define GATHER_VALUES /*Nothing*/
#define COMMA /*Nothing */
#include "inc_functions.h"
#include "inc_methods.h"
#include "inc_constructors.h"
#undef N_ARGS
#undef T_ARGS
#undef F_ARGS
#undef GATHER_VALUES

#undef COMMA
#define COMMA ,

/* Expand for 1 argument */
#define N_ARGS 1
#define T_ARGS class A1
#define F_ARGS A1
#define GATHER_VALUES Value<A1>::Get(L,1)
#include "inc_functions.h"
#include "inc_methods.h"
#include "inc_constructors.h"
#undef N_ARGS
#undef T_ARGS
#undef F_ARGS
#undef GATHER_VALUES

/* Expand for 2 arguments */
#define N_ARGS 2
#define T_ARGS class A1, class A2
#define F_ARGS A1,A2
#define GATHER_VALUES Value<A1>::Get(L,1), Value<A2>::Get(L,2)
#include "inc_functions.h"
#include "inc_methods.h"
#include "inc_constructors.h"
#undef N_ARGS
#undef T_ARGS
#undef F_ARGS
#undef GATHER_VALUES

/* Expand for 3 arguments */
#define N_ARGS 3
#define T_ARGS class A1, class A2, class A3
#define F_ARGS A1,A2,A3
#define GATHER_VALUES Value<A1>::Get(L,1), Value<A2>::Get(L,2), \
                      Value<A3>::Get(L,3)
#include "inc_functions.h"
#include "inc_methods.h"
#include "inc_constructors.h"
#undef N_ARGS
#undef T_ARGS
#undef F_ARGS
#undef GATHER_VALUES

/* Expand for 4 arguments */
#define N_ARGS 4
#define T_ARGS class A1, class A2, class A3, class A4
#define F_ARGS A1,A2,A3,A4
#define GATHER_VALUES Value<A1>::Get(L,1), Value<A2>::Get(L,2), \
                      Value<A3>::Get(L,3), Value<A4>::Get(L,4)
#include "inc_functions.h"
#include "inc_methods.h"
#include "inc_constructors.h"
#undef N_ARGS
#undef T_ARGS
#undef F_ARGS
#undef GATHER_VALUES

/* Expand for 5 arguments */
#define N_ARGS 5
#define T_ARGS class A1, class A2, class A3, class A4, class A5
#define F_ARGS A1,A2,A3,A4,A5
#define GATHER_VALUES Value<A1>::Get(L,1), Value<A2>::Get(L,2), \
                      Value<A3>::Get(L,3), Value<A4>::Get(L,4), \
                      Value<A5>::Get(L,5)
#include "inc_functions.h"
#include "inc_methods.h"
#include "inc_constructors.h"
#undef N_ARGS
#undef T_ARGS
#undef F_ARGS
#undef GATHER_VALUES

/* Expand for 6 arguments */
#define N_ARGS 6
#define T_ARGS class A1, class A2, class A3, class A4, class A5, class A6
#define F_ARGS A1,A2,A3,A4,A5,A6
#define GATHER_VALUES Value<A1>::Get(L,1), Value<A2>::Get(L,2), \
                      Value<A3>::Get(L,3), Value<A4>::Get(L,4), \
                      Value<A5>::Get(L,5), Value<A6>::Get(L,6)
#include "inc_functions.h"
#include "inc_methods.h"
#include "inc_constructors.h"
#undef N_ARGS
#undef T_ARGS
#undef F_ARGS
#undef GATHER_VALUES
/* undef , */
#undef COMMA

template<class F>
inline void PushFunction(lua_State *L, F f) {
  internal::PushRAWFunc(L, f, Function<F>::Call);
}


} // end of SLB3 namespace
#endif

