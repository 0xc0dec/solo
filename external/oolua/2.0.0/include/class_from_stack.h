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

/** \cond INTERNAL*/
/**
	@file class_from_stack.h
	\brief Retrieves a typed pointer from a potential userdata on the stack.
	\details Preforms configuration dependent checks in a attempt to return a
	typed pointer from a userdata which resides on the stack.
*/
/** \endcond */

#ifndef OOLUA_CLASS_FROM_STACK_H_
#	define OOLUA_CLASS_FROM_STACK_H_

#	include "lua_includes.h"
#	include "proxy_class.h"
#	include "proxy_userdata.h"
#	include "oolua_config.h"
#	include "lvd_type_traits.h"

#	include "proxy_base_checker.h"

#if OOLUA_DEBUG_CHECKS == 1
#	include <cassert>
#endif

//forward declare
struct lua_State;

namespace OOLUA
{

	/** \cond INTERNAL*/

	namespace INTERNAL
	{
		/**
			\brief Uses a config dependent check to verify "index" is a library created userdata
			\tparam T Requested type wanted from the stack
			\param[in] vm Lua virtual machine on which to operate
			\param[in] index Stack index at which the potential userdata is located
			\return T* Either a pointer of the requested type or NULL
		*/
		template<typename T>
		T* check_index(lua_State * vm, int index);

		/**
			\brief Uses config dependent checks to verify "index" is a library created userdata
			\details Preforms an extra check that the stack type is not constant, throws a Lua
			error if the type is.
			\tparam T Requested type wanted from the stack
			\param[in] vm Lua virtual machine on which to operate
			\param[in] index
			\return T* Either a pointer of the requested type or NULL
		*/
		template<typename T>
		T* check_index_no_const(lua_State * vm, int index);

		/**
			\brief Uses config dependent checks to verify "index" is a library created userdata
			\param[in] vm Lua virtual machine on which to operate
			\param[in] index Stack index to check
			\param[out] ud If verification is successful then it points to the userdata on the stack
			\return Boolean indicting if verification was successful
		*/
		bool index_is_userdata(lua_State* vm, int index, Lua_ud*& ud);
		/**
			\brief Generates a Lua error that the stack type was constant when a none constant was wanted
			\param[in] vm Lua virtual machine on which to operate
			\param[in] pulling_class_type
 		*/
		void report_error_userdata_is_constant(lua_State* const vm, char const* pulling_class_type);

		/**
			\brief blah
			\tparam T Requested base class wanted from the stack type
			\param[in] stack_ud
			\return T* Either a valid base pointer or NULL
			\pre T is not the same type as the type stack_ud represents
		*/
		template<typename T>
		T* valid_base_ptr_or_null(Lua_ud const* stack_ud);

#if OOLUA_CHECK_EVERY_USERDATA_IS_CREATED_BY_OOLUA == 0
		inline bool index_is_userdata(lua_State* vm, int index, Lua_ud*& ud)
		{
			ud = static_cast<Lua_ud *>(lua_touserdata(vm, index));
			return ud != 0;
		}
#endif

		/**
			\brief Checks the stack top is a valid index before calling check_index
			\param[in] vm Lua virtual machine on which to operate
			\return NULL when top is not a valid stack index else the result of check_index
		*/
		template<typename T>
		inline T* class_from_stack_top(lua_State * vm)
		{
			const int top = lua_gettop(vm);
			return top ? check_index<T>(vm, top) : NULL;
		}

		/**
			\brief Checks that the stack top is a valid index before calling check_index_no_const
			\param[in] vm Lua virtual machine on which to operate
			\return NULL when top is not a valid stack index else the result of check_index_no_const
		*/
		template<typename T>
		inline T* none_const_class_from_stack_top(lua_State * vm)
		{
			const int top = lua_gettop(vm);
			return top ? check_index_no_const<T>(vm, top) : NULL;
		}

		template<typename T>
		T* valid_base_ptr_or_null(Lua_ud const* stack_ud)
		{
#if OOLUA_USE_SHARED_PTR == 1
			Lua_ud requested_ud = {{0}, 0, &register_class_imp<T>, 0, 0};
#else
			Lua_ud requested_ud = {0, 0, &register_class_imp<T>, 0};
#endif
			stack_ud->base_checker(&requested_ud, stack_ud);
			return static_cast<T* >(requested_ud.void_class_ptr);
		}

		template<typename T>
		T* check_index(lua_State *  vm, int index)
		{
			Lua_ud * ud;
			if( !index_is_userdata(vm, index, ud))return 0;
			if( !ud_is_type<T>(ud))
			{
				return valid_base_ptr_or_null<T>(ud);
			}
			return static_cast<T* >(ud->void_class_ptr);
		}
#if OOLUA_USE_SHARED_PTR == 1

		namespace
		{
			template<typename T>
			struct pointer_type
			{
				typedef T* ptr;
				typedef T raw;
				enum {ud_flag = 0};
			};

			template<typename T>
			struct pointer_type<OOLUA_SHARED_TYPE<T> >
			{
				typedef OOLUA_SHARED_TYPE<T> ptr;
				typedef T raw;
				enum {ud_flag = SHARED_FLAG};
				typedef char never_call_with_const_type_always_raw_type[LVD::is_const<T>::value ? -1 : 1];
			};

			template<typename T>
			inline T** ud_member_cast(Lua_ud* ud, T*&)
			{
				return reinterpret_cast<T**>(&ud->void_class_ptr);
			}

			template<typename T, template <typename> class Shared_pointer_class>
			inline Shared_pointer_class<T>* ud_member_cast(Lua_ud* ud, Shared_pointer_class<T>&)
			{
				return reinterpret_cast<Shared_pointer_class<T>*>(ud->shared_object);
			}

			template<typename T>
			inline T* ptr_null(T**) // NOLINT(readability/casting)
			{
				return NULL;
			}

			template<typename T, template <typename> class Shared_pointer_class>
			inline Shared_pointer_class<T> ptr_null(Shared_pointer_class<T>*)
			{
				return Shared_pointer_class<T>();
			}
			template<typename T>
			inline void destroy_ud_ptr(T** /*ud_ptr*/) // NOLINT(readability/casting)
			{}

			template<typename T, template <typename> class Shared_pointer_class>
			inline void destroy_ud_ptr(Shared_pointer_class<T>* ud_ptr)
			{
				ud_ptr->~Shared_pointer_class<T>();
			}
		} // namespace //NOLINT(readability/namespace)

		template<typename T>
		struct stack_checker
		{
			typedef pointer_type<T> type;
			typedef typename type::ptr ptr_type;
			typedef typename type::raw raw_type;

			static ptr_type& valid_base_ptr_or_null(Lua_ud const* stack_ud, ptr_type& ptr )
			{
#if OOLUA_USE_SHARED_PTR == 1
				Lua_ud requested_ud = {{0}, 0, &register_class_imp<raw_type>, 0, type::ud_flag};
#else
				Lua_ud requested_ud = {0, 0, &register_class_imp<raw_type>, 0};
#endif
				stack_ud->base_checker(&requested_ud, stack_ud);

				//this may not be a void class pointer but if found it will have some
				//value other than NULL. C99 TR3 allows this type casting
				if (requested_ud.void_class_ptr)
				{
					ptr_type* result = ud_member_cast(&requested_ud, ptr);
					ptr = *result;
					destroy_ud_ptr(result);
				}
				return ptr;
			}
			static ptr_type check_index(lua_State * vm, int index)
			{
				Lua_ud * ud;
				ptr_type ptr(ptr_null(static_cast<ptr_type*>(NULL)));
				if( !index_is_userdata(vm, index, ud))return ptr;
				if( !ud_is_type<raw_type>(ud))
				{
					return valid_base_ptr_or_null(ud, ptr);
				}
				return *ud_member_cast(ud, ptr);
			}
			static ptr_type check_index_no_const(lua_State * vm, int index)
			{
				Lua_ud * ud;
				ptr_type ptr(ptr_null(static_cast<ptr_type*>(NULL)));
				if( !index_is_userdata(vm, index, ud))return ptr;
				if( userdata_is_constant(ud) )
				{
					report_error_userdata_is_constant(vm, OOLUA::Proxy_class<raw_type>::class_name);
					//does not return
				}
				if( !ud_is_type<raw_type>(ud))
				{
					return valid_base_ptr_or_null(ud, ptr);
				}
				return *ud_member_cast(ud, ptr);
			}
		};

#endif

		template<typename T>
		T* check_index_no_const(lua_State * vm, int index)
		{
			Lua_ud * ud;
			if( !index_is_userdata(vm, index, ud))return 0;
			if( userdata_is_constant(ud) )
			{
				report_error_userdata_is_constant(vm, OOLUA::Proxy_class<T>::class_name);
				//does not return
			}
			if( !ud_is_type<T>(ud))
			{
				return valid_base_ptr_or_null<T>(ud);
			}
			return static_cast<T* >(ud->void_class_ptr);
		}

	} // namespace INTERNAL // NOLINT
	/** \endcond */
} // namespace OOLUA

#endif //OOLUA_CLASS_FROM_STACK_H_
