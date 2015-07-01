/*
The MIT License

Copyright (c) 2009 - 2015 Liam Devine

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "proxy_storage.h"
#include "lua_includes.h"
#include "proxy_userdata.h"
#include "oolua_stack.h"


#if OOLUA_DEBUG_CHECKS == 1
#	include <cassert>
#endif

namespace OOLUA
{

	/** \cond INTERNAL*/
	namespace INTERNAL
	{

		//pushes the weak table on top and returns its absolute index
		//The weak table is a table in the Lua registry specific to OOLua,
		//which has void pointer keys and values of userdata pointers.
		int push_weak_table(lua_State* vm)
		{
			Weak_table::getWeakTable(vm);
			return lua_gettop(vm);
		}

		//if found it is left on the top of the stack and returns true
		//else the stack is same as on entrance to the function and false returned
		bool is_there_an_entry_for_this_void_pointer(lua_State* vm, void* ptr)
		{
			int wt = push_weak_table(vm);
			bool result = is_there_an_entry_for_this_void_pointer(vm, ptr, wt);
			lua_remove(vm, wt);
			return result;
		}

		bool is_there_an_entry_for_this_void_pointer(lua_State* vm, void* ptr, int tableIndex)
		{
			lua_pushlightuserdata(vm, ptr);//weakTable ... ptr
			lua_rawget(vm, tableIndex);//weakTable .... (full user data or nil)
			if( !lua_isnil(vm, -1) )
			{
				return true;//leave ud on top
			}
			lua_pop(vm, 1);//pop nil
			return false;
		}

		void add_ptr_if_required(lua_State* const vm, void* ptr, int ud_index, int weak_index)
		{
			lua_pushlightuserdata(vm, ptr);//ptr
			lua_rawget(vm, weak_index);//(null, ud or table)
			switch(lua_type(vm, -1))
			{
				case LUA_TNIL:
				{
					lua_pop(vm, 1);//pop the null
					lua_pushlightuserdata(vm, ptr);//key
					lua_pushvalue(vm, ud_index);//key ud
					lua_rawset(vm, weak_index);//table[key] = ud
					break;
				}
				case LUA_TUSERDATA:
				{
					//(weak_index ... ud_index ...) ud
					if( lua_rawequal(vm, -1, ud_index) == 1)
						lua_pop(vm, 1);//pop the ud
					else
					{
						lua_createtable(vm, 0, 2);////(weak_index ... ud_index ...) foundUd, collision_table
						lua_pushlightuserdata(vm, ptr);//key //(weak_index ... ud_index ...) foundUd, collisionTable, lightUd
						//lua_createtable(vm, 0, 2);//value
						lua_pushvalue(vm, -2); //(weak_index ... ud_index ...) foundUd, collisionTable, lightUd, collisionTable

						Weak_table::getCollisionMetatable(vm);// foundUd, collisionTable, lightUd, collisionTable, collisionMetaTable

						//setmetatable(collisionTable, collisionMetaTable)
						lua_setmetatable(vm, -2); // foundUd, collisionTable, lightUd, collisionTable

						lua_pushvalue(vm, ud_index);//key
						lua_pushvalue(vm, ud_index);//value
						//collisionTable[newLightUd] = newLightUd
						lua_rawset(vm, -3); // foundUd, collisionTable, lightUd, collisionTable

						lua_pushvalue(vm, -4);//key // foundUd, collisionTable, lightUd, collisionTable, foundUd
						lua_pushvalue(vm, -5);//value // foundUd, collisionTable, lightUd, collisionTable, foundUd, foundUd
						//collisionTable[foundUd] = foundUd
						lua_rawset(vm, -3);// foundUd, collisionTable, lightUd, collisionTable

						//weakTable[lightUd] = collisionTable
						lua_rawset(vm, weak_index);// foundUd, collisionTable,

						lua_pushboolean(vm, 1);// foundUd, collisionTable, boolean
						lua_rawset(vm, weak_index);// foundUd,
						//weakTable[collisionTable] = 1

						userdata_ptr_collision(static_cast<Lua_ud*>(lua_touserdata(vm, ud_index)));
						userdata_ptr_collision(static_cast<Lua_ud*>(lua_touserdata(vm, -1)));
						lua_pop(vm, 1);//pop ud that was not this ptr
					}
					break;
				}
				case LUA_TTABLE:
				{
					lua_pushvalue(vm, ud_index);
					lua_rawget(vm, -2);
					if(lua_type(vm, -1) != LUA_TNIL)
						lua_pop(vm, 2);//pop found ud and the collision table
					else
					{
						lua_pushvalue(vm, ud_index);//collisionTable, nil, ud
						lua_pushvalue(vm, ud_index);//collisionTable, nil, ud, ud
						//collisionTable[ud] = ud
						lua_rawset(vm, -4);//collisionTable, nil
						lua_pop(vm, 2); //pop nil and the collision table
						userdata_ptr_collision(static_cast<Lua_ud*>(lua_touserdata(vm, ud_index)));
					}
				}
			}
		}

		bool ud_at_index_is_const(lua_State* vm, int index)
		{
			return INTERNAL::userdata_is_constant( static_cast<Lua_ud *>( lua_touserdata(vm, index) ) );
		}

		Lua_ud* new_userdata(lua_State* vm, void* ptr, bool is_const
							, oolua_function_check_base base_checker
							, oolua_type_check_function type_check
							, oolua_release_shared_ptr shared_release)
		{
			Lua_ud* ud = static_cast<Lua_ud*>(lua_newuserdata(vm, sizeof(Lua_ud)));
			ud->flags = 0;
			reset_userdata(ud, ptr, is_const, base_checker, type_check, shared_release);
#if OOLUA_CHECK_EVERY_USERDATA_IS_CREATED_BY_OOLUA == 1 && OOLUA_USERDATA_OPTIMISATION == 1
			OOLUA_SET_COOKIE(ud->flags);
#endif
			return ud;
		}

		void reset_userdata(Lua_ud* ud, void* ptr, bool is_const
							, oolua_function_check_base base_checker
							, oolua_type_check_function type_check
							, oolua_release_shared_ptr shared_release)
		{
			ud->void_class_ptr = ptr;
			ud->base_checker = base_checker;
			ud->type_check = type_check;
#if OOLUA_USE_SHARED_PTR == 1
			ud->shared_ptr_release = shared_release;
#else
			(void)shared_release;
#endif
			userdata_const_value(ud, is_const);
		}

	} // namespace INTERNAL //NOLINT
	/**\endcond*/
} // namespace OOLUA
