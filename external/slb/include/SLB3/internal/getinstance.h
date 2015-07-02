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
#ifndef __SLB3_GET__
#define __SLB3_GET__ 1

#include <utility>
#include "../objectheader.h"

namespace SLB3 {
namespace internal {

  template<class T>
  std::pair<T*, bool> GetOrConvertPtr(lua_State *L, int index) {
    typedef Wrapper<T> W;
    typedef typename W::Policy P;

    bool is_const = true;
    T* result = NULL;
    int top = lua_gettop(L);

    Object object(lua_touserdata(L, index));
    if (object.header) {
      is_const = (object.header->type == kObject_Const_Ptr);
      if (object.header->unpack == P::Unpack) {
        result = reinterpret_cast<T*>(object.unpack());
      } else {
        result = reinterpret_cast<T*>(object.unpackByParent(W::GetName()));
      }
    }
    lua_settop(L, top);
    return std::make_pair(result, is_const);
  }
}

	template<class T>
  T* GetPtr(lua_State *L, int index) {
    typedef Wrapper<T> W;
    typedef typename W::Policy P;

    std::pair<T*,bool> result = internal::GetOrConvertPtr<T>(L, index);
    if (result.second) return NULL;
    return result.first;
  }
  
  template<class T>
  const T* GetConstPtr(lua_State *L, int index) {
    typedef Wrapper<T> W;
    typedef typename W::Policy P;

    std::pair<T*,bool> result = internal::GetOrConvertPtr<T>(L, index);
    return result.first;
  }

  template<class T>
  T* GetPtrOrError(lua_State *L, int index) {
    T* ptr = GetPtr<T>(L,index);
    if (!ptr) luaL_error(L, "Expected (non-const) Object %s", Wrapper<T>::GetName());
    return ptr;
  }

  template<class T>
  const T* GetConstPtrOrError(lua_State *L, int index) {
    const T* ptr = GetConstPtr<T>(L,index);
    if (!ptr) luaL_error(L, "Expected (const) Object %s", Wrapper<T>::GetName());
    return ptr;
  }


} // end of wrappers namespace

#endif
