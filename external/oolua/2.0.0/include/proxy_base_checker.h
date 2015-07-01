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
/**
	@file proxy_base_checker.h
	\brief Checks the hierarchical bases to ensure a cast is defined
	\details Walks a list of bases class defined in a OOLUA::Proxy_class
	to find if a type can be converted to the requested type, if it is valid
	then the procedures will preform the cast.
*/
#ifndef OOLUA_PROXY_BASE_CHECKER_H_
#	define OOLUA_PROXY_BASE_CHECKER_H_

#include "type_list.h"
#include "proxy_userdata.h"
#include "proxy_class.h"
#include "oolua_config.h"

#if OOLUA_USE_SHARED_PTR == 1
#	include OOLUA_SHARED_HEADER
#endif

struct lua_State;

namespace OOLUA
{
	/** \cond INTERNAL*/
	namespace INTERNAL
	{
		/*forward declare*/
		template<typename T>
		void register_class_imp(lua_State* vm);

		/**\addtogroup OOLuaClassTypeChecking
		@{*/
		/**
			\brief Start function which sets up the recursive base checking
			\tparam T The type whose bases will be checked
			\param[in,out] requested_ud The OOLua userdata type wanted
			\param[in] stack_ud The stack userdata type
			\pre requested_ud->void_class_pointer is NULL
			\post If the request type was a base then requested_ud->void_class_pointer is none NULL
			\details
			Starts the recursive iteration checks over the bases of T
			\note
			Neither this function or the recursive functions it calls set
			requested_ud->void_class_pointer to NULL, this happens in in
			OOLUA::INTERNAL::valid_base_ptr_or_null
		*/
		template<typename T>
		void requested_ud_is_a_base(Lua_ud* requested_ud, Lua_ud const* stack_ud);

		/**
			\brief Checks if the ud has the same type as tparam
			\tparam T The type to check the ud against
			\param[in] ud The OOLua userdata instance to check
			\result boolean indicating if the comparison is equal
		*/
		template<typename T>
		bool ud_is_type(Lua_ud const* ud);

		/**
			\struct CastToRequestedProxyType
			\brief Casts the stack type to the requested types
			\tparam ProxyStackType Correct type of the stack userdata before it was cast to void pointer
			\tparam BaseType The requested base type of the stack userdata
			\details Correctly casts the stack type to the requested type, taking into
			consideration class offsets.
		*/
		template<typename ProxyStackType, typename BaseType, int DoWork>
		struct CastToRequestedProxyType;

		/**
			\struct Is_a_base
			\brief Recurrsive base checker.
			\tparam ProxyStackType Correct type of the stack ud before it was cast to a void pointer.
			\tparam Bases Type list of base clases
			\tparam BaseIndex Index into the base class type list
			\tparam BaseType Current index type for the type list
			\details Iterates over the base classes and exits when it has either found the correct
			base type or has exhausted the base class types.

		*/
		template<typename ProxyStackType, typename Bases, int BaseIndex, typename BaseType>
		struct Is_a_base;

		template<typename ProxyStackType, typename BaseType, int DoWork = 1>
		struct CastToRequestedProxyType
		{
#if OOLUA_USE_SHARED_PTR == 1
			/*
				If the type in the stack_ud is a shared pointer you can request a shared pointer
				or a normal pointer from it which will be stored in requested_ud.

				If the type in the stack_ud is not a shared pointer it is undefined to
				request a shared pointer from it.
			*/
			static void cast(INTERNAL::Lua_ud * requested_ud, INTERNAL::Lua_ud const* stack_ud)
			{
				stack_ud->flags & SHARED_FLAG ?
						do_shared_cast(requested_ud, const_cast<Lua_ud*>(stack_ud))
						: do_ptr_cast(requested_ud, stack_ud);
			}

			static void do_ptr_cast(INTERNAL::Lua_ud * requested_ud, INTERNAL::Lua_ud const* stack_ud)
			{
				//cast the class void ptr from the stack to the stacktype
				//then to base type to get correct offset
				requested_ud->void_class_ptr = static_cast<BaseType*>(static_cast<typename ProxyStackType::class_* >(stack_ud->void_class_ptr));
			}

			static void do_shared_cast(INTERNAL::Lua_ud * requested_ud, INTERNAL::Lua_ud * stack_ud)
			{
				//cast the generic shared ptr to the the stacktype shared_ptr
				OOLUA_SHARED_TYPE<typename ProxyStackType::class_>* stack_shared_ptr = reinterpret_cast<OOLUA_SHARED_TYPE<typename ProxyStackType::class_>* >(stack_ud->shared_object);

				//the type wanted is a shared pointer
				if(requested_ud->flags & SHARED_FLAG)
				{
					//worried that this may get optimised away by clang, I recall something like this
					//in an old mailing list thread!
					//construct a base class shared ptr using placement new
					new(requested_ud->shared_object) OOLUA_SHARED_TYPE<BaseType>(*stack_shared_ptr);
				}
				else //the type wanted is a normal pointer
				{
					//cast from stack type to base type and store in requested_ud
					requested_ud->void_class_ptr = static_cast<BaseType*>(stack_shared_ptr->get());
				}
			}
#else
			static void cast(INTERNAL::Lua_ud * requested_ud, INTERNAL::Lua_ud const* stack_ud)
			{
				//cast the class void ptr from the stack to the stacktype
				//then to base type to get correct offset
				requested_ud->void_class_ptr = static_cast<BaseType*>(static_cast<typename ProxyStackType::class_* >(stack_ud->void_class_ptr));
			}
#endif
		};

		template<typename ProxyStackType, typename Bases, int BaseIndex, typename BaseType>
		struct Is_a_base
		{
			void operator()(Lua_ud* __restrict requested_ud, INTERNAL::Lua_ud const* __restrict stack_ud)
			{
				//is this a base
				if( ud_is_type<BaseType>(requested_ud) )
				{
					CastToRequestedProxyType<ProxyStackType, BaseType, 1>::cast(requested_ud, stack_ud);
					return;
				}
				//check the next in the type list
				Is_a_base<
					ProxyStackType
					, Bases
					, BaseIndex + 1
					, typename TYPELIST::At_default< Bases, BaseIndex + 1, TYPE::Null_type >::Result
				> nextIsBase;
				nextIsBase(requested_ud, stack_ud);
			}
		};

		template<typename ProxyStackType, typename Bases, int BaseIndex>
		struct Is_a_base<ProxyStackType, Bases, BaseIndex, TYPE::Null_type>
		{
			void operator()(INTERNAL::Lua_ud* __restrict/*requested_ud*/
							, INTERNAL::Lua_ud const* __restrict/*stack_ud*/)
			{}//noop
		};

		template<typename T>
		inline void requested_ud_is_a_base(Lua_ud* __restrict requested_ud, Lua_ud const* __restrict stack_ud)
		{
			INTERNAL::Is_a_base<OOLUA::Proxy_class<T>
					, typename OOLUA::Proxy_class<T>::AllBases
					, 0
					, typename TYPELIST::At_default< typename OOLUA::Proxy_class<T>::AllBases, 0, TYPE::Null_type >::Result
				> checkBases;
			checkBases(requested_ud, stack_ud);
		}

		template<typename T>
		inline bool ud_is_type(Lua_ud const* ud)
		{
			//some compilers need a hand it also enforces that the typedef in oolua_userdata is correct
			return ud->type_check ==  static_cast<oolua_type_check_function>(&register_class_imp<T>);
		}
		/**@}*/
	} // namespace INTERNAL // NOLINT
	/** \endcond  INTERNAL */
} // namespace OOLUA

#endif
