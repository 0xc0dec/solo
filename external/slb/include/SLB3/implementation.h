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
#ifndef __SLB3_IMPLEMENTATION__
#define __SLB3_IMPLEMENTATION__ 1

#include "declaration.h"
#include "internal/class_imp.h"
#include "internal/pod_imp.h"

#define SLB3_IMPLEMENTATION(Type, VarName) \
  void SLB3::Wrapper<Type>::Register(SLB3::internal::Class<Type> &VarName)

// Starts the definition of a POD-like structure. For every element of the
// structure use 
//    SLB3_POD_PARAMETER(NameMember)
// Example:
//
//   SLB3_POD_DECLARATION(MyStruct,
//     SLB3_POD_MEMBER(width)
//     SLB3_POD_MEMBER(height)
//     SLB3_POD_MEMBER(depth)
//    )
//

#define SLB3_POD_IMPLEMENTATION(Name, ...) \
  SLB3_INTERNAL_POD_GET_IMPLEMENTATION(Name, __VA_ARGS__) \

#endif
