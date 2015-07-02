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
#ifndef __SLB3_INTERNAL_POD_IMP__
#define __SLB3_INTERNAL_POD_IMP__ 1

#define SLB3_INTERNAL_POD_GET_IMPLEMENTATION(NAME, CODE) \
void SLB3_POD_Get(lua_State *L, int index, NAME *data) {\
  if (!lua_istable(L,index)) {\
    luaL_error(L, "Expected table at index %d", index);\
  }\
  index = lua_absindex(L, index);\
  int stack_save = lua_gettop(L);\
  /* go through all elements*/ \
  lua_pushnil(L);\
  while(lua_next(L,index)) {\
    if (lua_isstring(L,-2)) {\
      const char *key = lua_tostring(L,-2);\
      CODE;\
      luaL_error(L, "Invalid key to member '%s'", key);\
    } else {\
      luaL_error(L, "Object-table with non-string key invalid");\
    }\
  next:\
    lua_pop(L,1);\
  }\
  lua_settop(L, stack_save);\
}

#define SLB3_POD_MEMBER(NAME) \
  if (strcmp(key, #NAME) == 0) { \
    SLB3::GetInto(L, -1, &(data->NAME));\
    goto next;\
  }


#endif
