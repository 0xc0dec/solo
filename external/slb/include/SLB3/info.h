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

#ifndef __SLB3_INFO__
#define __SLB3_INFO__

#include "config.h"

namespace SLB3 {

  struct BaseInfo {
    enum Type {
      kType_unknown,
      kType_void,
      kType_primitive_value,
      kType_class,
      kType_function,
    };
    virtual Type type() const = 0;
    virtual const char* name() const = 0;
    virtual int num_args() const { return -1; }
    virtual const BaseInfo** args() const { return NULL; }
    virtual const BaseInfo* type_returned() const { return NULL; }
  };

  const BaseInfo* GetInfo(lua_State *, int index);

  namespace internal {
    // Pushes an upvalue, with extra information to ensure later than GetInfo
    // can work properly.
    void PushInfoUpvalue(lua_State *, const BaseInfo *);
    void RegisterInfoClass(lua_State *, int class_meta, const BaseInfo *);
  }

#if SLB3_ENABLE_INFO_GENERATION 

  template<class T>
  struct InfoCompilerName {
    static const char *Name() {
    #if defined(_MSC_VER)                                                        
      return __FUNCTION__; 
    #elif defined(__GNUC__) || defined(__SNC__)                                  
      return __PRETTY_FUNCTION__;
    #else                                                                        
      return "<unknown>";  
    #endif
    }
  };

  template<class T>
  struct Info : public BaseInfo {
    Type type() const { return  kType_unknown; }
    const char* name() const { return InfoCompilerName<T>::Name(); }
  };

  template<>
  struct Info<void> : public BaseInfo {
    Type type() const { return  kType_void; }
    const char* name() const { return "void"; }
  };

  #define PRIMITIVE_VALUE(T) \
    template<>\
    struct Info<T> : public BaseInfo {\
      Type type() const { return kType_primitive_value; } \
      const char* name() const { return #T; }\
    };\
    template<>\
    struct Info<T*> : public BaseInfo {\
      Type type() const { return kType_primitive_value; } \
      const char* name() const { return #T" *"; }\
    };\
    template<>\
    struct Info<const T*> : public BaseInfo {\
      Type type() const { return kType_primitive_value; } \
      const char* name() const { return "const "#T" *"; }\
    };\
    template<>\
    struct Info<T&> : public BaseInfo {\
      Type type() const { return kType_primitive_value; } \
      const char* name() const { return #T" &"; }\
    };\
    template<>\
    struct Info<const T&> : public BaseInfo {\
      Type type() const { return kType_primitive_value; } \
      const char* name() const { return "const "#T" &"; }\
    };\

  PRIMITIVE_VALUE(float)
  PRIMITIVE_VALUE(double)
  PRIMITIVE_VALUE(char)
  PRIMITIVE_VALUE(short)
  PRIMITIVE_VALUE(int)
  PRIMITIVE_VALUE(long)
  PRIMITIVE_VALUE(unsigned char)
  PRIMITIVE_VALUE(unsigned short)
  PRIMITIVE_VALUE(unsigned int)
  PRIMITIVE_VALUE(unsigned long)

  #undef PRIMITIVE_VALUE

  namespace internal {

  template<class T1 = void, class T2 = void, class T3 = void, class T4 = void,
           class T5 = void, class T6 = void, class T7 = void, class T8 = void,
           class T9 = void, class T10 = void, class T11 = void , class T12 = void>
   struct InfoArray {
     static inline const BaseInfo** Compute() {
       static Info<T1> t1;
       static Info<T2> t2;
       static Info<T3> t3;
       static Info<T4> t4;
       static Info<T5> t5;
       static Info<T6> t6;
       static Info<T7> t7;
       static Info<T8> t8;
       static Info<T9> t9;
       static Info<T10> t10;
       static Info<T11> t11;
       static Info<T12> t12;
       static const BaseInfo * array[] = {
          &t1, &t2, &t3, &t4, &t5, &t6, &t7, &t8, &t9, &t10, &t11, &t12,
          NULL};
       return array;
      }
   };
  } // internal namespace

  #define SLB3_INTERNAL_INFO_DECLARATION(T_Type, Name) \
    namespace SLB3 {\
    template<>\
    struct Info<T_Type> : public BaseInfo {\
      BaseInfo::Type type() const { return BaseInfo::kType_class; } \
      const char* name() const { return Name; }\
    };\
    template<>\
    struct Info<T_Type*> : public BaseInfo {\
      BaseInfo::Type type() const { return BaseInfo::kType_class; } \
      const char* name() const { return Name" *"; }\
    };\
    template<>\
    struct Info<const T_Type*> : public BaseInfo {\
      BaseInfo::Type type() const { return BaseInfo::kType_class; } \
      const char* name() const { return "const "Name" *"; }\
    };\
    template<>\
    struct Info<T_Type&> : public BaseInfo {\
      BaseInfo::Type type() const { return BaseInfo::kType_class; } \
      const char* name() const { return Name" &"; }\
    };\
    template<>\
    struct Info<const T_Type&> : public BaseInfo {\
      BaseInfo::Type type() const { return BaseInfo::kType_class; } \
      const char* name() const { return "const "Name" &"; }\
    };\
    } // end of SLB3 namespace

#else
  #define SLB3_INTERNAL_INFO_DECLARATION(Type, Name) /*..nothing..*/
#endif // if SLB3_ENABLE_INFO_GENERATION

}

#endif // __SLB3_INFO__
