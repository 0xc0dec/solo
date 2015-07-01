/**
	\file oolua.h
	\brief Kitchen sink header file for Object Oriented Lua. Which could be a good
	candidate for a PCH.
*/

/**
	\mainpage
	\section OOLuaIntroduction Introduction
	OOLua is cross platform, test driven, dependency free and Open Source library. It uses C++03
	template meta-programming and pre-processor magic internally, which can be used to generate
	non intrusive proxies that provide a fast bridge for the interaction of C++ classes and functions
	with Lua.
	<p>
	The library provides multiple inheritance C++ classes without using C++'s RTTI,
	can be compiled either with or without exception support and is easily configurable.
	OOLua also provides a thin and simple abstraction layer for interfacing with the Lua stack and
	types in a type safe manner, whilst also supporting a means to bypass the type safety using the Lua
	light userdata type. The library can be use in a pick and mix fashion or in its entirety, whilst
	still using the Lua C API.

	OOLua is developed by Liam Devine who has over ten years experience using the Lua programming
	language, having what he considers to be a deep understanding of both it and also C++.

	This is not a fully original work and was originally based on ideas from binding classes using
	the famous <a href="http://lua-users.org/wiki/CppBindingWithLunar" target="_blank">Lunar</a>
	and <a href="http://www.lua.org/notes/ltn005.html" target="_blank">Lua Technical Note 5</a>.

	\section OOLuaCompatibility Lua compatibility
	This version of the library is compatible with the following Lua implementations
		\li Rio Lua 5.1, 5.2 and 5.3 http://www.lua.org
		\li LuaJIT 1.1.8 and 2.0 http://www.luajit.org/

	\section OOLuaLinks Links
		\li Project Home  https://oolua.org
		\li Library documentation https://docs.oolua.org/2.0.0
		\li Issue tracker https://oolua.org/issues
		\li Mailing list https://oolua.org/mailinglist

	\section OOLuaLicence Licence
	OOLua:
	\copyright
	The MIT License\n
	Copyright (c) 2009 - 2015 Liam Devine\n
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:\n
 	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.\n\n
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.\n

	Loki Type lists:
	\copydetails type_list.h

	Luna :
	\copydetails oolua_registration.h
*/


#ifndef OOLUA_H_
#	define OOLUA_H_


#	include "lua_includes.h"
#	include "oolua_dsl.h"
#	include "proxy_function_exports.h"
#	include "oolua_version.h"
#	include "oolua_error.h"
#	include "oolua_stack.h"
#	include "oolua_script.h"
#	include "oolua_open.h"
#	include "oolua_chunk.h"
#	include "oolua_registration.h"
#	include "oolua_table.h"
#	include "oolua_ref.h"
#	include "oolua_helpers.h"

/**
	\namespace OOLUA
	\brief This is the root namespace of the Library
	\details There are sub namespaces contained in OOLUA yet mostly these are not
	meant for general usage, instead this namespace contains all Public API functions,
	structures etc.
*/
namespace OOLUA
{
	/**
		\brief Helper function to set a Lua global variable.
		\tparam T Type for instance
		\param[in] vm lua_State
		\param[in] name Global name to set
		\param[in] instance Any variable which is valid to push to the stack
		\return Boolean indicating if the operation was successful \see OOLuaErrorReporting
	*/
	template<typename T>
	bool set_global(lua_State* vm, char const* name, T& instance)
	{
		if ( !OOLUA::push(vm, instance) ) return false;
		lua_setglobal(vm, name);
		return true;
	}

	/**
		\brief None template version
		\details Enables setting a global with a value of lua_CFunction without
		requiring you make a reference to the function.
		\param[in] vm The lua_State to work on
		\param[in] name String which is used for the global name
		\param[in] instance The lua_CFuntion which will be set at the global value for name
	*/
	bool set_global(lua_State* vm, char const* name, lua_CFunction instance);

	/**
		\brief Helper function to set a Lua global variable to nil.
		\param[in] vm lua_State
		\param[in] name Global name to set
	*/
	void set_global_to_nil(lua_State* vm, char const * name);

	/**
		\brief Helper function to set a Lua global variable.
		\tparam T Type for instance
		\param[in] vm lua_State
		\param[in] name Global name to query
		\param[out] instance Any variable which is valid to pull from the stack
		\return Boolean indicating if the operation was successful \see OOLuaErrorReporting
	*/
	template<typename T>
	bool get_global(lua_State* vm, char const* name, T& instance)
	{
		lua_getglobal(vm, name);
		return OOLUA::pull(vm, instance);
	}

} // namespace OOLUA

#endif //OOLUA_H_
