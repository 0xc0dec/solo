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
	\file oolua_function.h
	\brief Provides the class OOLUA::Lua_function which is a helper for calling
	Lua functions.
*/

#ifndef OOLUA_FUNCTION_H_
#	define OOLUA_FUNCTION_H_

#	include "lua_includes.h"
#	include "oolua_stack_fwd.h"
#	include "oolua_ref.h"
#	include "oolua_boilerplate.h"
#	include <string>

namespace OOLUA
{

/** \cond INTERNAL */
#if OOLUA_USE_EXCEPTIONS == 1
#	define OOLUA_CALLER_TRY \
	try \
	{
#	define OOLUA_CALLER_HANDLE_FAIL \
	} \
	catch(...) \
	{ \
		lua_settop(m_lua, top); \
		throw; \
	} \
	return false; /*Shhhhhhhhhh*/
#else
#	define OOLUA_CALLER_TRY
#	define OOLUA_CALLER_HANDLE_FAIL \
	lua_settop(m_lua, top); \
	return false;
#endif

/**
	\addtogroup OOLuaGeneratorTemplates
	@{
*/
/**
	\brief Generates an OOLUA::Lua_function call operator
	\hideinitializer
	\param NUM Number of parameters for which it the template will generate the operator for
*/
#define OOLUA_INTERNAL_FUNC_OPERATOR(NUM) \
	template<typename FUNC_TYPE OOLUA_COMMA_PREFIXED_TYPENAMES_##NUM > \
	bool operator()(FUNC_TYPE const& func OOLUA_FCALL_PARAM_##NUM) \
	{ \
		int error_index(0); \
		const int top = get_top(); \
		OOLUA_CALLER_TRY \
		if( prep_function(func, NUM, error_index) \
				OOLUA_FCALL_PUSH_##NUM \
		) return call(NUM, error_index); \
		OOLUA_CALLER_HANDLE_FAIL \
	}
/**@}*/
/** \endcond */





/**
	\struct Lua_function
	\brief Structure which is used to call a Lua function.
	\details
	Calling a Lua function, from C++ code using OOLua's API, can be achieved using a
	\ref OOLUA::Lua_function "Lua_function" object. This is a state bound caller,
	and the state in which the callee will be invoked is specified either in the
	\ref OOLUA::Lua_function::Lua_function(lua_State*) "constructor" or via the
	\ref OOLUA::Lua_function::bind_script "bind_script" member function.

	To invoke a callee, the \ref OOLUA::Lua_function type uses a call operator. The operator's
	first parameter must be the callee and it can be specified using one of the following types:
		\li std::string A function in the bound state's global table
		\li \ref OOLUA::Lua_func_ref A reference to a function
		\li int A valid stack index
	If the callee is identified via a valid stack index, then this index will remain on
	the stack at same absolute location after the caller has returned.

 	The call operator is also overloaded to enable the passing of parameters to the callee;
 	the maximum number of parameters is defined by the configurable value
 	\ref OOLuaConfigLuaParams "\"lua_params\"".
*/
	struct Lua_function
	{
		/** \brief Default constructor initialises the object
			\post Any call to a function call operator will cause
			an error until a lua_State is bound via \ref bind_script
		*/
		Lua_function();
		/** \brief Binds the state vm to this instance */
		Lua_function(lua_State* vm); // NOLINT Let OOLUA::Script decay to pointer

		/**@{*/
		/**
			\brief Function call operator
			\return Result indicating success
			\tparam FUNC_TYPE
			\see \ref OOLuaErrorReporting
			*/
			OOLUA_INTERNAL_FUNC_OPERATORS
		/**@}*/

		/** \brief Sets the state in which functions will be called
			\param[in] vm The state to bind to the instance.
		*/
		void bind_script(lua_State* const vm);
	private:
		int get_top();
		bool call(int const nparams, int const error_index);
		bool prep_function(Lua_func_ref const& func, int const nparams, int& error_index);
		bool prep_function(std::string const& func, int const nparams, int& error_index);
		bool prep_function(int const func, int const nparams, int& error_index);
		lua_State* m_lua;
	};

} // namespace OOLUA

#undef OOLUA_CALLER_TRY
#undef OOLUA_CALLER_HANDLE_FAIL

#endif //OOLUA_FUNCTION_H_
