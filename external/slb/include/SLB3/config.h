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
#ifndef __SLB3_CONFIG__
#define __SLB3_CONFIG__ 1

// include lua
extern "C" {
	#include <lauxlib.h>
	#include <lualib.h>
	#include <lua.h>
}

// Object-header Type:
// it must hold a value of the ObjectType (0-3)
#ifndef SLB3_OBJECT_HEADER_TYPE_SIZE_IN_BITS
  #define SLB3_OBJECT_HEADER_TYPE_SIZE_IN_BITS 2
#endif

// Object-header flags:
// It can be used by your policies to flag objects
// and act as padding for memory alignment
#ifndef SLB3_OBJECT_HEADER_FLAGS_SIZE_IN_BITS
  #define SLB3_OBJECT_HEADER_FLAGS_SIZE_IN_BITS 30
#endif

// SLB3_CACHE_BASE_CLASS_METHODS -- Speeds ups method search of base classes
//   If enabled when accessing a valid parent method, the search
//   will be cached in the object's metatable for future uses.
#ifndef SLB3_CACHE_BASE_CLASS_METHODS
  #define SLB3_CACHE_BASE_CLASS_METHODS 1
#endif

// Enables Generation of info, to dynamically query info about classes/functions
#ifndef SLB3_ENABLE_INFO_GENERATION
  #define SLB3_ENABLE_INFO_GENERATION 1
#endif

#endif
