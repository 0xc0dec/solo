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
#ifndef __SLB3_OBJECT_HEADER__
#define __SLB3_OBJECT_HEADER__

#include "config.h"
#include <cstring>

namespace SLB3 {

	template<class T>
	struct Wrapper {
    /* Default - Invalid Wrapper */
    typedef typename T::This_Type_Is_Not_Wrapped_So_It_Cant_Be_Used Policy;
	};

	enum ObjType {
		kObject_Invalid,
		kObject_Instance,
		kObject_Ptr,
		kObject_Const_Ptr
	};

  struct Object;
  typedef void* (*UnpackFunc)(const Object&);

  // -- to define internal::ParentRef class
  namespace internal {
    typedef void* (*ConversionFunc_T)(void*);
    struct ParentRef {
      const char *parent_name;
      UnpackFunc unpack;
      ConversionFunc_T convertFrom; // converts from child to parent
    };
    // Called to retrieve the Conversions array for a given type, based on the
    // unpack func pointer as type-id. Will always be NULL terminated.
    const ParentRef* GetParentsConversionArray(
        lua_State *L,
        const UnpackFunc type_unpack_func);
  }

	struct ObjHeader {
		unsigned type  :  SLB3_OBJECT_HEADER_TYPE_SIZE_IN_BITS;
		unsigned flags :  SLB3_OBJECT_HEADER_FLAGS_SIZE_IN_BITS;
    UnpackFunc unpack;

    // NULL terminated parent references by unpack_func, do not use directly
    // This is used internally to adapt pointers of instances to their parent
    // references
    const internal::ParentRef *parents;
	};

  struct Object {
    ObjHeader* const header;
    void* const data;

    Object(): header(NULL), data(NULL) {}
    explicit Object(void *raw_ptr)
        : header(reinterpret_cast<ObjHeader*>(raw_ptr))
        , data(raw_ptr?
            reinterpret_cast<char*>(raw_ptr)+sizeof(ObjHeader)
           :NULL) {}

    void* unpack() const { return header->unpack(*this); }

    // Transform current object to a instance of the given parent, using stored
    // conversions functions (if exists) or returns NULL
    void* unpackByParent(const char *parent_name) const {
      void* const base = unpack();
      if (base && header->parents) { // if object can't be unpacked, we skip the search
        const internal::ParentRef *pref = header->parents;
        for( ;pref->unpack != NULL; ++pref) {
          if (strcmp(parent_name, pref->parent_name) == 0) {
            return (*pref->convertFrom)(base);
          }
        }
      }

      return NULL;
    }
  };

  template<class T>
  inline T* Cast(const Object &obj) {
    if (obj.data && obj.header->type != kObject_Const_Ptr) {
      assert(obj.header->unpack == Wrapper<T>::Policy::Unpack);
      return reinterpret_cast<T*>(obj.unpack());
    }
    return NULL;
  }

  template<class T>
  inline const T* ConstCast(const Object &obj) {
    if (obj.data) {
      assert(obj.header->unpack == Wrapper<T>::Policy::Unpack);
      return reinterpret_cast<const T*>(obj.unpack());
    }
    return NULL;
  }


}

#endif
