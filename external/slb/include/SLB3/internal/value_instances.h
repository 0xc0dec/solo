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
#ifndef __SLB3_VALUE_INSTANCES__
#define __SLB3_VALUE_INSTANCES__

#include "getinstance.h"

namespace SLB3 {

  /* General note, we always need to push first the metatable, and then the
     object to make sure the metatable is completely registered first (for
     example, to know in advance class parents before pushing the new intance)
     
     for that we do this:
        internal::PushMetatable<T>(L);
        Wrapper<T>::Policy::PushPtr(L, obj);
        lua_insert(L,-2);
        lua_setmetatable(L,-2);
     looks weird, but is the way we are sure the table is registered first
     */


  // Default implementation
  template<class T>
  struct Value {
    typedef T ClassType;
    typedef const T& GetType;
    static void Push(lua_State *L,const T &src) {
      void *mem = NULL;
      internal::PushMetatable<T>(L);
      Object holder = Wrapper<T>::Policy::NewInstanceBegin(L, &mem);
      lua_insert(L,-2);
      lua_setmetatable(L,-2);
      // Call copy constructor, with new-in-place 
      T* new_obj = new (mem) T(src);
      Wrapper<T>::Policy::NewInstanceEnd(holder, new_obj);
    }
    static const T& Get(lua_State *L, int index) {
      return *GetConstPtrOrError<T>(L, index);
    }
  };

  template<class T>
  struct Value<T*> {
    typedef T ClassType;
    typedef T* GetType;
    static void Push(lua_State *L, T *obj) {
      if (obj) {
        internal::PushMetatable<T>(L);
        Wrapper<T>::Policy::PushPtr(L, obj);
        lua_insert(L,-2);
        lua_setmetatable(L,-2);
      } else {
        lua_pushnil(L); // NULL == nil in lua
      }
    }
    static T* Get(lua_State *L, int index) {
      return GetPtr<T>(L, index);
    }
  };
  
  template<class T>
  struct Value<const T*> {
    typedef T ClassType;
    typedef const T* GetType;
    static void Push(lua_State *L, const T *obj) {
      internal::PushMetatable<T>(L);
      Wrapper<T>::Policy::PushConstPtr(L, obj);
      lua_insert(L,-2);
      lua_setmetatable(L,-2);
    }
    static const T* Get(lua_State *L, int index) {
      return GetConstPtr<T>(L, index);
    }
  };

  template<class T>
  struct Value<const T&> {
    typedef T ClassType;
    typedef const T& GetType;
    static void Push(lua_State *L,const T &obj) {
      internal::PushMetatable<T>(L);
      Wrapper<T>::Policy::PushConstPtr(L, &obj);
      lua_insert(L,-2);
      lua_setmetatable(L,-2);
    }
    static const T& Get(lua_State *L, int index) {
      return *GetConstPtrOrError<T>(L, index);
    }
  };
  
  template<class T>
  struct Value<T&> {
    typedef T ClassType;
    typedef T& GetType;
    static void Push(lua_State *L,T &obj) {
      internal::PushMetatable<T>(L);
      Wrapper<T>::Policy::PushPtr(L, &obj);
      lua_insert(L,-2);
      lua_setmetatable(L,-2);
    }
    static T& Get(lua_State *L, int index) {
      return *GetPtrOrError<T>(L, index);
    }
  };
}

#endif
