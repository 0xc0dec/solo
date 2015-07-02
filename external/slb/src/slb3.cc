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

#include <SLB3/declaration.h>
#include <SLB3/extra/stackguard.h>
#include <cstring>

using SLB3::extra::StackGuard;

namespace SLB3 {
namespace internal {

// Gets or creates a new table nested into a previous table
static void GetOrCreateNestedTable(lua_State *L, int index, const char *name,
    unsigned name_size) {
  char buff[256];
  memcpy(buff, name, name_size);
  buff[name_size] = '\0';

  assert(lua_istable(L, index) && "Expected table at index");
  index = lua_absindex(L, index);
  lua_getfield(L, index, buff);
  if (!lua_istable(L, -1)) {
    lua_pop(L,1); // remove the nil/invalid value
    lua_newtable(L);
    lua_pushstring(L, buff);
    lua_pushvalue(L,-2); // copy of the new table
    lua_rawset(L, index);
  }
}


//-----------------------------------------------------------------------------
// TODO: Review this diagram, is not acurated to new usage of Objects with a
//       pointer to the parent's table conversion
//-----------------------------------------------------------------------------
//      ... <-- tables (Namespace) <-- G_ (Global enviroment)   
//       | 
//  _____v________   meta    __________
// | Class Table |--------->| __index -|-> InheritanceCallback
// | ..methods.. |<-\       | __call   |       ^
// | ..values... |   \       ----------(C)     | (upvalue 1)
//  -------------(B)  \                       /
//  _____              \                     /  
// | obj | Obj's meta  |                    /
//  -----\___________  |                   /
//       | __index --|-/                  /
//       | __call    |                   /
//       | __gc      |                  /
//       | __parents-|----> __________ /   
//        -----------(A)   | [p_meta]-|--->func to transform (C* -> P*)
//                         | [p_meta] |              
//                         | .........|              
//                          ----------(D) 
//   (A) Object's metatable
//   (B) Object's methods/Values
//   (C) (B)'s metatable -> triggered when no method was found, and constructors
//   (D) table to all parents metatable (like A)
//-----------------------------------------------------------------------------

static int InheritanceCallback(lua_State *L) {
  // arg0 = table /ignore/
  // arg1 = name of the method

  // go through all tables
  lua_pushnil(L); // first key
  while(lua_next(L,lua_upvalueindex(1))) {
    // [+1] key = parent meta table (D)
    // [+1] value = function to transform child to parent 
    lua_getfield(L,-2,"__index"); // [+1] parent's methods 
    lua_pushvalue(L,2); // [+1] key 
    lua_rawget(L,-2); // [-1,+1] search for method
    if (!lua_isnil(L,-1)) {
    #ifdef SLB3_CACHE_BASE_CLASS_METHODS
      // method found, keep a copy in the methods table for faster
      // future accesses
      lua_pushvalue(L,2);
      lua_pushvalue(L,-2);
      lua_rawset(L,1);
    #endif
		// return the function found
		return 1;
	}
    lua_pop(L, 3); //
  }
  return 0;
}

void private_AddInheritance(
    lua_State *L,
    int class_meta,
    UnpackFunc objects_unpack,
    UnpackFunc parents_unpack,
    ConversionFunc_T conversion_func) {

  class_meta = lua_absindex(L, class_meta);
  int parent_meta = lua_gettop(L);
  assert(lua_istable(L,parent_meta) && 
      "Invalid call to addInheritance, top is not a metatable of parent class");

  /*--------------------------------------------------------------------------*/
  // This is related to OnClassDefinitionEnds method.
  // Collect unpack functions into __parents_unpack_funcs
  lua_pushlightuserdata(L, (void*)objects_unpack);
  lua_setfield(L, class_meta, "__object_unpack");
  lua_getfield(L, class_meta, "__parents_unpack_funcs");
  lua_getfield(L, parent_meta, "__class");
  ParentRef *parent_ref = (ParentRef*) lua_newuserdata(L, sizeof(ParentRef));
  parent_ref->parent_name = lua_tostring(L,-2);
  parent_ref->unpack = parents_unpack;
  parent_ref->convertFrom = conversion_func;
  lua_rawset(L,-3);
  lua_pop(L,1); // remove __parents_unpack_funcs table
  /*--------------------------------------------------------------------------*/

  lua_getfield(L, class_meta, "__parents"); 
  if (lua_isnil(L,-1)) {
    // first parent class added, create a new __parents table
    lua_pop(L,1);
    lua_newtable(L); // new table for __parents

    // set the table in class_meta[__parents] = t
    lua_pushvalue(L,-1);
    lua_setfield(L, class_meta, "__parents");

    // create the InheritanceCallback as __index meta-method of method's
    lua_getfield(L, class_meta, "__index");
    // get __index metatable of (class_meta)
    lua_getmetatable(L,-1);
    assert(lua_istable(L,-1) && "__index doesn't have metatable...");
    lua_pushvalue(L, -3); // __parents table
    lua_pushcclosure(L, InheritanceCallback, 1);
    lua_setfield(L,-2, "__index");
    lua_setmetatable(L,-2);
    lua_pop(L,1);
  }
  assert((lua_gettop(L) == parent_meta+1) && "Invalid top");
  assert(lua_istable(L,-1) && "Invalid table");

  lua_pushvalue(L, parent_meta); // [key]
  ConversionFunc_T *func = (ConversionFunc_T*)
      lua_newuserdata(L, sizeof(ConversionFunc_T)); // [value]
  *func = conversion_func;
  lua_rawset(L,-3); // set __parents[p_metatable] = conversion_func

  lua_settop(L,parent_meta-1);
}

void private_SetCallMetamethod(lua_State *L, int meta, lua_CFunction f) {
  int top = lua_gettop(L);
  lua_getfield(L, meta, "__index");
  lua_getmetatable(L,-1);
  assert(lua_istable(L,-1) && "__index table doesn't have metatable");
  lua_pushcclosure(L, f, 0);
  lua_setfield(L,-2, "__call");
  lua_pop(L,2);
  assert(top == lua_gettop(L) && "something went wrong");
}

} // end of internal namespace


//-- Namespace -----------------------------------------------------------------
Namespace::Namespace(lua_State *L, const char *name) : Table() {
  const int top = lua_gettop(L);
  unsigned last_namespace_start = 0;
  const unsigned name_len = strlen(name);

  // starting with the global namespace keep going through tables 
  lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
  for(unsigned i = 0 ; i < name_len; ++i) {
    if (name[i] == '.') {
      internal::GetOrCreateNestedTable(L, -1, name+last_namespace_start
          , i-last_namespace_start);
      lua_replace(L, -2);
      last_namespace_start = i+1;
    }
  }
  // last bit of the namespace
  if (name_len - last_namespace_start > 0) {
    internal::GetOrCreateNestedTable(L,-1, name+last_namespace_start,
      name_len - last_namespace_start);
    lua_replace(L, -2);
  }
  Table::init(L, Table::kTable_at_top);

  assert(lua_gettop(L) == top && "Error creating namespace");
}

Table::Table() : L_(NULL), registry_ref_(LUA_NOREF), parent_(NULL) {}

Table::Table(lua_State *L, const Table::Action a)
    : L_(NULL)
    , registry_ref_(LUA_NOREF)
    , parent_(NULL) {
  init(L, a);
}

Table::Table(Table *parent)
    : L_(NULL)
    , registry_ref_(LUA_NOREF)
    , parent_(parent) {
  init(parent->L_, kNew_table);
}


void Table::init(lua_State *L, const Table::Action a) {
  assert(L_ == NULL && "Initialized twice");
  L_ = L;
  switch(a) {
    case kNew_table:
      lua_newtable(L);
      break;
    case kTable_at_top:
      assert(lua_istable(L,-1) && "Expected table at top, to create Table obj");
      break;
  }
  // reference the table for future use
  registry_ref_ = luaL_ref(L, LUA_REGISTRYINDEX);
}

Table::~Table() {
  if (L_) luaL_unref(L_, LUA_REGISTRYINDEX, registry_ref_);
}

void Table::push(lua_State *L) const {
  if (L == L_) {
    lua_rawgeti(L_, LUA_REGISTRYINDEX, registry_ref_);
  } else {
    assert(!"Can't push to another lua_State different from "
        "where the table was created");
  }
}

void Table::rawSet(const char *name) {
  int top = lua_gettop(L_);
  lua_rawgeti(L_, LUA_REGISTRYINDEX, registry_ref_);
  lua_insert(L_, -2);
  lua_setfield(L_,-2, name);
  lua_pop(L_,1);
  assert(lua_gettop(L_) == top -1 && "Invalid rawSet");
}

Table Table::subtable(const char *name) {
  Table t(this);
  t.push(L_);
  rawSet(name);
  return t;
}

Table& Table::close() {
  assert(parent_ && "Error, this table can't be closed");
  return *parent_;
}

Table::Table(const Table& t)
    : L_(NULL)
    , registry_ref_(LUA_NOREF)
    , parent_(t.parent_) {
  t.push(t.L_);
  init(t.L_, kTable_at_top);
}

//----------------------------------------------------------------------------//
// Info methods
//----------------------------------------------------------------------------//
namespace internal {
  struct InfoUpvalue {
    void *ptr_to_GetInfo;
    const BaseInfo *info;
  };

  void PushInfoUpvalue(lua_State *L, const BaseInfo *info) {
    InfoUpvalue *upvalue = (InfoUpvalue*)lua_newuserdata(L, sizeof(InfoUpvalue));
    upvalue->ptr_to_GetInfo = (void*)GetInfo;
    upvalue->info = info;
  }

  void RegisterInfoClass(lua_State *L, int meta_, const BaseInfo *info) {
    StackGuard stack_guard(L);
    lua_getfield(L, LUA_REGISTRYINDEX, "SLB3_Classes");
    if (lua_isnil(L,-1)) {
      lua_pop(L,1);
      lua_newtable(L);
      lua_pushvalue(L,-1);
      lua_setfield(L, LUA_REGISTRYINDEX, "SLB3_Classes");
    }
    lua_getfield(L, meta_, "__index");
    lua_setfield(L, -2, info->name());
    lua_pop(L,1);

    lua_pushlightuserdata(L, (void*)info);
    lua_setfield(L, meta_, "__info");
  }
}

const BaseInfo* GetInfo(lua_State *L, int index) {
  const BaseInfo *result = NULL; 
  const int lua_top = lua_gettop(L);
  int type = lua_type(L, index);
  switch(type) {
    case LUA_TFUNCTION:
      // Funtions use upvalue 2 to store the BaseInfo
      if(lua_getupvalue(L,index,2)) {
        // Check is user data, and also the size matches our internal struct
        if (lua_isuserdata(L,-1) &&
            lua_rawlen(L,-1) == sizeof(internal::InfoUpvalue)) {
          // Convert to our struct, check the pointer
          const internal::InfoUpvalue *up =
              (const internal::InfoUpvalue*) lua_touserdata(L,-1);
          if (up && up->ptr_to_GetInfo == (void*)GetInfo) {
            result = up->info;
          }
        }
      }
    break;
  }
  lua_settop(L, lua_top); // restore stack
  return result;
}

//----------------------------------------------------------------------------//
// Helper functions to push/get arrays of primitive types
//----------------------------------------------------------------------------//
namespace internal {

int CheckPrimitivePtrEqual(lua_State *L) {
  void* v1 = *((void**) lua_touserdata(L, 1));
  void* v2 = *((void**) lua_touserdata(L, 2));
  lua_pushboolean(L,(v1 == v2)?1:0);
  return 1;
}

void  ValuePrimitivePtrPushMetatable(lua_State *L, const char *T_name) {
  if (luaL_newmetatable(L, T_name)) {
    static const char const_prefix[] = "SLB3_const_ptr_";
    int count = 1;
    // Add the type 
    lua_pushstring(L, T_name);
    lua_rawseti(L,-2, count++);
    // Check if is the const_version of the type
    if (strncmp(const_prefix, T_name, sizeof(const_prefix)-1)) {
      // if not const, then add const versions
      lua_pushstring(L, const_prefix);
      lua_pushstring(L, T_name+sizeof("SLB3_ptr_")-1);
      lua_concat(L,2);
      lua_rawseti(L,-2, count++);
      // also conversion to void*
      if (strcmp(T_name, "SLB3_ptr_void")) {
        lua_pushstring(L, "SLB3_ptr_void");
        lua_rawseti(L, -2, count++);
      }
    }
    // Always convertible to const void*
    if (strcmp(T_name, "SLB3_const_ptr_void")) {
      lua_pushstring(L, "SLB3_const_ptr_void");
      lua_rawseti(L, -2, count++);
    }
    // add an __eq metamethod
    lua_pushcfunction(L, CheckPrimitivePtrEqual);
    lua_setfield(L,-2, "__eq");
  }
}

void* ValuePrimitivePtrGetVoidPtr(lua_State *L, const char *T_name, int index) {
  void* result = NULL;
  index = lua_absindex(L,index);
  int top = lua_gettop(L);
  lua_getmetatable(L, index);
  if (lua_istable(L,-1)) {
    // Check if is a instance of an object
    lua_getfield(L,-1, "__class");
    if (!lua_isnil(L,-1)) {
      Object obj(lua_touserdata(L, index));
      // Check if can be converted to void*, or const void*
      if (strcmp("SLB3_ptr_void", T_name) == 0) {
        // Ensure integrity by checking it is NOT a const ptr
        if (obj.header->type != kObject_Const_Ptr) {
          result = obj.header->unpack(obj);
        }
      } else if (strcmp("SLB3_const_ptr_void", T_name) == 0) {
        result = obj.header->unpack(obj);
      }
      goto found;
    }
    // Is not a class Asume is a pointer to a primitive type
    lua_pop(L,1);
    unsigned t_len = lua_rawlen(L,-1);
    for(unsigned i = 1; i <= t_len; ++i) {
      lua_rawgeti(L, -1, i);
      if (strcmp(lua_tostring(L,-1), T_name) == 0) {
        result = *((void**)lua_touserdata(L, index));
        goto found;
      }
      lua_pop(L,1);
    }
  }
found:
  lua_settop(L, top);
  return result;
}


lua_Integer GetInteger(lua_State *L, int index) {
  lua_Integer result = 0;
  int is_num = 0;
  result = lua_tointegerx(L, index, &is_num);
  if (!is_num) {
    // ... try to see if the element at index is an enum
    if(luaL_getmetafield(L, index, "is_enum")) {
      lua_pop(L,1);
      lua_Integer *value = (lua_Integer*) lua_touserdata(L, index);
      result = *value;
    }
  }
  return result;
}

int CheckEnumEqual(lua_State *L) {
  lua_Integer *v1 =  (lua_Integer*) lua_touserdata(L, 1);
  lua_Integer *v2 =  (lua_Integer*) lua_touserdata(L, 2);
  if (*v1 == *v2) {
    lua_pushboolean(L,1);
    return 1;
  }
  return 0;
}

void PushEnumMetatable(lua_State *L, const char *enum_name) {
  if(luaL_newmetatable(L, enum_name)) {
    lua_pushboolean(L,true);
    lua_setfield(L,-2, "is_enum");
    lua_pushcfunction(L, CheckEnumEqual);
    lua_setfield(L, -2, "__eq");
  }
}

int CheckInstanceEqual(lua_State *L) {
  Object obj1(lua_touserdata(L,1)); 
  Object obj2(lua_touserdata(L,2)); 

  if (obj1.unpack() == obj2.unpack()) {
    lua_pushboolean(L,1);
    return 1;
  }
  return 0;
}

int lua_meta__void_ptr(lua_State *L) {
  Object obj(lua_touserdata(L,1));
  lua_pushlightuserdata(L, obj.unpack());
  return 1;
}


void OnClassDefinitionBegins(lua_State *L, int meta_index) {
  // Create __eq method
  lua_pushcfunction(L, CheckInstanceEqual);
  lua_setfield(L, meta_index, "__eq");
  lua_newtable(L);
  lua_setfield(L, meta_index, "__parents_unpack_funcs");
  // Add __void_ptr function
  lua_pushcfunction(L, lua_meta__void_ptr);
  lua_setfield(L, meta_index, "__void_ptr");
}

// Pops a value from the stack as the array to convert child->parent classes
void RegisterParentsConversionArray( lua_State *L, UnpackFunc child) {
  int ptable = lua_gettop(L);
  assert(lua_isuserdata(L, ptable) && "Invalid call");
  lua_getfield(L, LUA_REGISTRYINDEX, "SLB3_ParentsConversionArray");
  if (lua_isnil(L,-1)) {
    lua_pop(L,1);
    lua_newtable(L);
    lua_pushvalue(L,-1);
    lua_setfield(L, LUA_REGISTRYINDEX, "SLB3_ParentsConversionArray");
  }
  lua_pushlightuserdata(L, (void*) child);
  lua_pushvalue(L, ptable);
  lua_rawset(L,-3);
  lua_settop(L, ptable-1); // consume the table
}

void OnClassDefinitionEnds(lua_State *L, int meta_index) {
  int top = lua_gettop(L);
  lua_getfield(L, meta_index, "__object_unpack");
  if (!lua_isnil(L,-1)) {
    void *object_unpack_ptr = lua_touserdata(L,-1);
    // Get __parents_unpack_funcs (which should be an table) and replace it
    // with the structure that holds child->parent conversions
    lua_getfield(L, meta_index, "__parents_unpack_funcs");
    int dict = lua_gettop(L);
    assert(lua_istable(L, dict) && "Mmm...looks like class was defined twice");
    // Get the current number of elements to build a fixed size table
    size_t length = 1; // always have +1 elements (the null termination)
    lua_pushnil(L);
    while(lua_next(L,dict)) {
      length = length+1;
      lua_pop(L,1);
    }
    assert(length >= 2 && "Invalid length");
    // Reserve the table
    ParentRef *parents_table = (ParentRef*) lua_newuserdata(L, length*sizeof(ParentRef));
    RegisterParentsConversionArray(L, (UnpackFunc)object_unpack_ptr);
    // fill the table
    size_t index = 0;
    lua_pushnil(L);
    while(lua_next(L,dict)) {
      assert((index < length-1) && "Something went wrong filling parents table");
      parents_table[index] = *((ParentRef*) lua_touserdata(L,-1));
      lua_pop(L,1); // drop value (name)
      index++;
    }
    parents_table[index].unpack = NULL;
    parents_table[index].convertFrom = NULL;
    assert(index == length-1 && "Index not at end of the table, which is wrong");
  }
  // remove __parents_unpack_funcs and __object_unpack
  lua_pushnil(L);
  lua_setfield(L, meta_index, "__object_unpack");
  lua_pushnil(L);
  lua_setfield(L, meta_index, "__parents_unpack_funcs");
  lua_settop(L,top);
}

const ParentRef* GetParentsConversionArray(
    lua_State *L,
    const UnpackFunc type_unpack_func) {
  int top = lua_gettop(L);
  const ParentRef *result = NULL;
  lua_getfield(L, LUA_REGISTRYINDEX, "SLB3_ParentsConversionArray");
  if (!lua_isnil(L,-1)) {
    lua_pushlightuserdata(L, (void*)type_unpack_func);
    lua_rawget(L,-2);
    result = (const ParentRef*) lua_touserdata(L,-1);
  }
  // return
  lua_settop(L,top);
  return result;
}

} // internal
//----------------------------------------------------------------------------//

} // end of SLB3 Namespace
