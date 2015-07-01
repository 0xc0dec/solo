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
	\file proxy_public_member.h
	\brief Proxies a class public member variable
*/

#ifndef OOLUA_PROXY_PUBLIC_MEMBER_H_
#	define OOLUA_PROXY_PUBLIC_MEMBER_H_

#include "oolua_stack_fwd.h"
#include "proxy_test.h"
#include "lvd_type_traits.h"
/** \cond INTERNAL */


namespace OOLUA
{
	namespace INTERNAL
	{
		template<typename T, int byValue, int hasProxyType>
		struct shouldPushValueByReference
		{
			enum {value = 0};
			static void push(lua_State* vm, T* input)
			{
				OOLUA::push(vm, *input);
			}
		};

		/*Not by value and has a proxy type*/
		template<typename T>
		struct shouldPushValueByReference<T, 0, 1>
		{
			enum {value = 0};
			static void push(lua_State* vm, T* input)
			{
				if(*input) OOLUA::push(vm, *input); else lua_pushnil(vm);
			}
		};

		/*by value and has a proxy type*/
		template<typename T>
		struct shouldPushValueByReference<T, 1, 1>
		{
			enum {value = 1};
			static void push(lua_State* vm, T* input)
			{
				OOLUA::push(vm, input);
			}
		};

#if OOLUA_USE_SHARED_PTR == 1
		template<typename T>
		struct shouldPushValueByReference<OOLUA_SHARED_TYPE<T>, 1, 0>
		{
			enum {value = 0};
			static void push(lua_State* vm, OOLUA_SHARED_TYPE<T>* input)
			{
				if(*input) OOLUA::push(vm, *input); else lua_pushnil(vm);
			}
		};
#endif

		struct PushPublicMember
		{
			template<typename T>
			static void push(lua_State* vm, T* input)
			{
				shouldPushValueByReference<T,
					//can not have a pointer to reference.
					//T will either be a pointer to type
					//or type
					LVD::by_value<T>::value
					, has_a_proxy_type<typename LVD::raw_type<T>::type >::value >
						::push(vm, input);
			}
		};

		struct GetPublicMember
		{
			template<typename T>
			static void get(lua_State* vm, T* output)
			{
				get_imp(vm, output
					, LVD::Int2type<
						LVD::by_value<T>::value
						&& has_a_proxy_type<typename LVD::raw_type<T>::type>::value
					>()
				); // NOLINT
			}
			template<typename T>
			static void get_imp(lua_State* vm, T* output, LVD::Int2type<0>)
			{
				OOLUA::INTERNAL::LUA_CALLED::get(vm, 2, *output);
			}
			template<typename T>
			static void get_imp(lua_State* vm, T* output, LVD::Int2type<1>)
			{
				T* temp(0);
				OOLUA::INTERNAL::LUA_CALLED::get(vm, 2, temp);
				*output = *temp;
			}
		};

	} // namespace INTERNAL // NOLINT
} // namespace OOLUA


#define OOLUA_MGET_INTERNAL_2(PublicInstance, GetterName) \
	int GetterName(lua_State* vm) const \
	{ \
		OOLUA::INTERNAL::PushPublicMember::push(vm, &m_this->PublicInstance); \
		return 1; \
	}

#define OOLUA_MGET_INTERNAL_1(PublicInstance) \
	OOLUA_MGET_INTERNAL_2(PublicInstance, get_##PublicInstance)


#define OOLUA_MSET_INTERNAL_2(PublicInstance, SetterName) \
	int SetterName(lua_State* vm) \
	{ \
		OOLUA::INTERNAL::GetPublicMember::get(vm, &m_this->PublicInstance); \
		return 0; \
	}

#define OOLUA_MSET_INTERNAL_1(PublicInstance) \
	OOLUA_MSET_INTERNAL_2(PublicInstance, set_##PublicInstance)


#define OOLUA_MGET_MSET_INTERNAL_3(PublicInstance, GetterName, SetterName) \
	OOLUA_MGET_INTERNAL_2(PublicInstance, GetterName) \
	OOLUA_MSET_INTERNAL_2(PublicInstance, SetterName)

#define OOLUA_MGET_MSET_INTERNAL_1(PublicInstance) \
	OOLUA_MGET_INTERNAL_1(PublicInstance) \
	OOLUA_MSET_INTERNAL_1(PublicInstance)

	/** \endcond */

#endif //OOLUA_PROXY_PUBLIC_MEMBER_H_

