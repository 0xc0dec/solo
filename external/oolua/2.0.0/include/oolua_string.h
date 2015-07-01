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
	\file oolua_string.h
	\brief Provides a method of not including the string header in DSL header
	files and allows other string types to be easily integrated.
*/
#ifndef OOLUA_STRING_H_
#	define OOLUA_STRING_H_

#include "lvd_type_traits.h"
#include "proxy_test.h"

struct lua_State;

namespace OOLUA
{
	/**
	\addtogroup OOLuaStringConfig
	@{
		\brief Enables a string type to be treated as an integral.
		\details String configuration enables a type to be treated as an alias and
		integral type for a Lua string, LUA_TSTRING. When a type is considered an
		integral it means that a heap allocation, for the object itself, does not occur
		and the instance is a value rather than a reference. The alternative to this,
		is for the string type to be proxied using the library's DSL.

		Identification of the type is tested using the compile time value contained in
		\ref OOLUA::STRING::is_integral_string_class "is_integral_string_class".
		A provided macro, \ref OOLUA_CLASS_OR_BASE_CONTAINS_METHOD, helps to create a
		structure that identifies the type and can be used in is_integral_string_class.
		The library file oolua_string.h contains examples of using this macro to
		identify a number of string types; such as std::string, C++03 and C++11
		strings(std::string, std::wstring, std::u16string and std::u32string), wxString
		and QString.

		Once a type can be identified in
		\ref OOLUA::STRING::is_integral_string_class "is_integral_string_class" as an
		integral string alias, then a user should provide implementations for the three
		specific templated functions, (OOLUA::STRING::push, OOLUA::STRING::pull and
		OOLUA::STRING::get) in oolua_string.cpp.

		\namespace OOLUA::STRING
		\brief Defines which type of string classes can be pulled and pushed from
		the stack with the public API and the DSL.
		\details
		I would really like to be able to forward declare string types in a cross
		platform way. For example when using GCC we could, but really shouldn't,
		use bits/stringfwd.h. However this is just not possible. Instead this file and
		its accompanying source file, oolua_string.cpp, provide a way to not include the
		string header through out the library DSL headers, yet still be able to use a
		string type when needed. It also facilitates the addition, and therefore
		interaction, of other string types with Lua.
		To allow a string type to be compatible with OOLua requires that three functions
		are defined for the type(push, pull and get) and is_integral_string_class::value
		must be a compile time value that returns one for the type.
	@}
	*/
	namespace STRING
	{
		/**
		\addtogroup  OOLuaStringConfig
		@{*/
		/**
		\brief Function to which public API calls resolve to.
		*/
		template<typename StringType>
		bool push(lua_State* const vm, StringType const& value);

		/**
		\brief Function to which public API calls resolve to.
		*/
		template<typename StringType>
		bool pull(lua_State* const vm, StringType& value);

		/**
		\brief Internal function used by the DSL to retrieve a string from the stack.
		\details The are a couple of differences between this function and
		\ref OOLUA::STRING::pull "pull". Firstly, pull retrieves the stack's top entry and
		pops it where as this function uses a stack index to identify the stack slot
		and the function does not pop the entry. Secondly, there is a difference in how
		errors are reported. Pull will either store and error or throw an exception,
		where as this function will eventually call lua_error.
		*/
		template<typename StringType>
		void get(lua_State* const vm, int idx, StringType &  value);
		/**@}*/
	} // namespace STRING //NOLINT(readability/namespace)
} // namespace OOLUA


namespace OOLUA
{
	namespace STRING
	{
/**
	\addtogroup  OOLuaStringConfig
	@{
	\def OOLUA_CLASS_OR_BASE_CONTAINS_METHOD
	\brief Creates a structure that enables checking a class type for a specific
	function signature that has a specific name.
	\hideinitializer
	\param StructName The name of the structure this macro will create.
	\param MethodSignature The signature the checker will look for.
	\param MethodName The function's name which has the MethodSignature
	@}
*/
#define OOLUA_CLASS_OR_BASE_CONTAINS_METHOD(StructName, MethodSignature, MethodName) \
		template<typename Type> \
		struct StructName \
		{ \
			typedef char yes[1]; \
			typedef char no [2]; \
			template<typename FuncT, FuncT> struct checker; \
			template <typename U> \
			static yes &check(checker<MethodSignature, &U::MethodName>*); \
			template <typename U> \
			static no &check(...); \
			enum {value = sizeof(check<Type>(0)) == sizeof(yes)}; \
		};

/**
	\addtogroup  OOLuaStringConfig
	@{
	\struct OOLUA::STRING::only_std_string_conforming_with_c_str_method
	\brief
	Defines the structure which checks for the method "c_str" which conforms to
	the std::string signature.
	@}
*/
OOLUA_CLASS_OR_BASE_CONTAINS_METHOD(only_std_string_conforming_with_c_str_method
								, char const* (U::*)()const
								, c_str)

/*this will pass for all std:: string types (string, wstring, u16string and u32string)*/
/*
OOLUA_CLASS_OR_BASE_CONTAINS_METHOD(std_conforming_with_c_str_method
								, typename U::const_pointer (U::*)()const
								, c_str)
*/

/*this will pass for all std:: string types (string wstring u16string u32string) and wxString*/
/*
OOLUA_CLASS_OR_BASE_CONTAINS_METHOD(std_conforming_with_substr_method
								, U(U::*)(typename U::size_type,typename U::size_type)const
								, substr)
*/

/*
	wxString
	bool ToCULong (unsigned long *val, int base=10) const
*/
/*
OOLUA_CLASS_OR_BASE_CONTAINS_METHOD(is_wxstring
								, (bool(U::*)(unsigned long*,int)const)
								, ToCULong)
*/

/*
	QString
	QString toCaseFolded () const
*/
/*
OOLUA_CLASS_OR_BASE_CONTAINS_METHOD(is_qstring
								, U(U::*)()const
								, toCaseFolded);
*/

		/**
		\addtogroup  OOLuaStringConfig
		@{
			\struct OOLUA::STRING::is_integral_string_class
			\brief Preforms the check on the type without including the string header
			\details To add a different string class type, see the commented out macros
			in oolua_string.h.
			\see \ref OOLUA_STD_STRING_IS_INTEGRAL
		@}
		*/
		template<typename T>
		struct is_integral_string_class
		{
			typedef typename LVD::raw_type<T>::type raw_type;
			enum { value =
#if OOLUA_STD_STRING_IS_INTEGRAL == 1
						INTERNAL::has_a_proxy_type<raw_type>::value == 0
						&& only_std_string_conforming_with_c_str_method<raw_type>::value == 1
#else
						0
#endif
				};
		};
	} // namespace STRING //NOLINT(readability/namespace)
} // namespace OOLUA

#endif //OOLUA_STRING_H_
