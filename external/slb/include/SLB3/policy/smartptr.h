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
#ifndef __SLB3_POLICY_SMARTPTR__
#define __SLB3_POLICY_SMARTPTR__ 1

#include "common.h"
#include <cstdlib> //< required to use malloc

namespace SLB3 {
namespace policy {

// TODO: Add allocator/deallocator class to be used to remplate malloc/free
// Of the new instance created.

template<class T>
struct MallocAllocator {
  static void* alloc() { return malloc(sizeof(T)); }
};

// This is an example of smartPointer
template<
    template <class> class T_SmartUsed,
    template<class> class T_AllocatorUsed = MallocAllocator>
struct SmartPtr {

template<class T>
struct Implementation {
  typedef T_SmartUsed<T> SmartT;
  typedef T_AllocatorUsed<T> AllocatorT;

  static void* Unpack(const Object &obj) {
    switch(obj.header->type) {
      case kObject_Invalid: assert(!"Invalid object"); return NULL;
      case kObject_Ptr:
      case kObject_Instance:
        return reinterpret_cast<SmartT*>(obj.data)->get();
      case kObject_Const_Ptr:
        return *reinterpret_cast<T**>(obj.data);
    }
    assert(!"Invalid policy::Struct::Unpack call");
    return NULL;
  }

  static Object NewInstanceBegin(lua_State *L, void **raw_mem) {
    Object obj = PushRawObject<T>(L, kObject_Instance, sizeof(SmartT));
    *raw_mem = AllocatorT::alloc(); // alloc call
    return obj;
  }

  static void NewInstanceEnd(const Object obj, T* new_obj) {
    assert(obj.header->type == kObject_Instance && "Invalid instance");
    new (obj.data) SmartT(new_obj);
  }


  static void PushPtr(lua_State *L, T* src) {
    Object obj = PushRawObject<T>(L, kObject_Ptr, sizeof(SmartT));
    new (obj.data) SmartT(src);
  }

  static void PushConstPtr(lua_State *L, const T* src) {
    Object obj = PushRawObject<T>(L, kObject_Const_Ptr, sizeof(const T*));
    *reinterpret_cast<const T**>(obj.data) = src;
  }

  static void Destructor(const Object obj) {
    if(obj.header->type == kObject_Instance || obj.header->type == kObject_Ptr) {
      SmartT *smart_ptr = reinterpret_cast<SmartT*>(obj.data);
      smart_ptr->~SmartT();
    }
  }
};
};

} // end of policy naespace
} // end of SLB3 namespace

#endif

