#pragma once

#include <tuple>
#include <string>
#include <functional>
#include <memory>
#include <vector>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace inclua
{
	namespace traits
	{
		template <typename T>
		struct Arity
		{
			static const int value = 1;
		};

		template <typename... Vs>
		struct Arity<std::tuple<Vs...>>
		{
			static const int value = sizeof...(Vs);
		};

		template <>
		struct Arity<void>
		{
			static const int value = 0;
		};

		template <std::size_t... Is>
		struct Indices
		{
		};

		template <std::size_t N, std::size_t... Idxs>
		struct Indexer : Indexer<N - 1, N - 1, Idxs...>
		{
		};

		template <std::size_t... Idxs>
		struct Indexer<0, Idxs...>
		{
			using Type = Indices<Idxs...>;
		};

		template <typename T>
		struct Id
		{
		};

		template <typename T>
		struct Primitive
		{
			static const bool value = false;
		};

		template <>
		struct Primitive<bool>
		{
			static const bool value = true;
		};

		template <>
		struct Primitive<lua_Number>
		{
			static const bool value = true;
		};

		template <>
		struct Primitive<lua_Integer>
		{
			static const bool value = true;
		};

		template <>
		struct Primitive<lua_Unsigned>
		{
			static const bool value = true;
		};

		template <>
		struct Primitive<std::string>
		{
			static const bool value = true;
		};
	}

	template <class>
	class LuaFunction
	{
	};

	namespace stack
	{
		template <typename T>
		inline T* get(traits::Id<T*>, lua_State* lua, int index)
		{
			return *reinterpret_cast<T **>(lua_topointer(lua, index));
		}

		template <typename T>
		inline T& get(traits::Id<T&>, lua_State* lua, int index)
		{
			static_assert(!traits::Primitive<T>::value, "Non-primitive type expected");
			return **reinterpret_cast<T **>(lua_topointer(lua, index));
		}

		inline bool get(traits::Id<bool>, lua_State* lua, int index)
		{
			return lua_toboolean(lua, index) != 0;
		}

#if LUA_VERSION_NUM >= 503
		inline int get(traits::Id<int>, lua_State* lua, int index)
		{
			return static_cast<int>(lua_tointeger(lua, index));
		}

		inline unsigned get(traits::Id<unsigned>, lua_State* lua, int index)
		{
			return static_cast<unsigned>(lua_tointegerx(lua, index, nullptr));
		}

		inline lua_Integer get(traits::Id<lua_Integer>, lua_State* lua, int index)
		{
			return lua_tointeger(lua, index);
		}

		inline lua_Unsigned get(traits::Id<lua_Unsigned>, lua_State* lua, int index)
		{
			return lua_tointegerx(lua, index, nullptr);
		}
#else
		inline int get(traits::Id<int>, lua_State* lua, int index)
		{
			return lua_tointeger(lua, index);
		}

		inline unsigned get(traits::Id<unsigned>, lua_State* lua, int index)
		{
			return lua_tounsigned(lua, index);
		}
#endif

		inline lua_Number get(traits::Id<lua_Number>, lua_State* lua, int index)
		{
			return lua_tonumber(lua, index);
		}

		inline std::string get(traits::Id<std::string>, lua_State* lua, int index)
		{
			size_t size;
			auto buff = lua_tolstring(lua, index, &size);
			return std::string{ buff, size };
		}

		template <typename T>
		inline T* getChecked(traits::Id<T*>, lua_State* lua, int index)
		{
			return *(T **)lua_topointer(lua, index);
		};

		template <typename T>
		inline T& getChecked(traits::Id<T&>, lua_State* lua, int index)
		{
			static_assert(!traits::Primitive<T>::value, "Non-primitive type expected");
			return **(T **)lua_topointer(lua, index);
		};

#if LUA_VERSION_NUM >= 503
		inline int getChecked(traits::Id<int>, lua_State* lua, int index)
		{
			return static_cast<int>(luaL_checkinteger(lua, index));
		}

		inline unsigned getChecked(traits::Id<unsigned>, lua_State* lua, int index)
		{
			return static_cast<unsigned>(luaL_checkinteger(lua, index));
		}

		inline lua_Integer getChecked(traits::Id<lua_Integer>, lua_State* lua, int index)
		{
			return luaL_checkinteger(lua, index);
		}

		inline lua_Unsigned getChecked(traits::Id<lua_Unsigned>, lua_State* lua, int index)
		{
			return luaL_checkinteger(lua, index);
		}
#else
		inline int getChecked(traits::Id<int>, lua_State* lua, int index)
		{
			return luaL_checkint(lua, index);
		}

		inline unsigned getChecked(traits::Id<unsigned>, lua_State* lua, int index)
		{
			return luaL_checkunsigned(lua, index);
		}
#endif

		inline lua_Number getChecked(traits::Id<lua_Number>, lua_State* lua, int index)
		{
			return luaL_checknumber(lua, index);
		}

		inline bool getChecked(traits::Id<bool>, lua_State* lua, int index)
		{
			return lua_toboolean(lua, index) != 0;
		}

		inline std::string getChecked(traits::Id<std::string>, lua_State* lua, int index)
		{
			size_t size;
			auto buff = luaL_checklstring(lua, index, &size);
			return std::string{ buff, size };
		}

		template <typename R, typename...Args>
		inline LuaFunction<R(Args ...)> getChecked(traits::Id<LuaFunction<R(Args ...)>>, lua_State* lua, int index)
		{
			lua_pushvalue(lua, index);
			return LuaFunction<R(Args ...)>{luaL_ref(lua, LUA_REGISTRYINDEX), lua};
		}

		template <typename R, typename... Args>
		inline LuaFunction<R(Args ...)> get(traits::Id<LuaFunction<R(Args ...)>> id, lua_State* lua, int index)
		{
			return getChecked(id, lua, index);
		}

		template <typename... Ts, std::size_t... N>
		static std::tuple<Ts...> popAndResetImpl(lua_State* lua, traits::Indices<N...>)
		{
			return std::make_tuple(stack::get(traits::Id<Ts> {}, lua, N + 1)...);
		}

		template <typename... Ts>
		inline std::tuple<Ts...> popAndReset(lua_State* lua)
		{
			std::tuple<Ts...> ret = popAndResetImpl<Ts...>(lua, typename traits::Indexer<sizeof...(Ts)>::Type());
			lua_settop(lua, 0);
			return ret;
		}

		inline void popAndReset(lua_State* lua)
		{
			lua_settop(lua, 0);
		}

		template <typename T>
		T pop(traits::Id<T> t, lua_State* lua)
		{
			T ret = get(t, lua, -1);
			lua_pop(lua, 1);
			return ret;
		}

		inline void push(lua_State*)
		{
		}

		inline void push(lua_State* lua, bool b)
		{
			lua_pushboolean(lua, b);
		}

		inline void push(lua_State* lua, int i)
		{
			lua_pushinteger(lua, i);
		}

		inline void push(lua_State* lua, unsigned u)
		{
#if LUA_VERSION_NUM>= 503
			lua_pushinteger(lua, u);
#else
			lua_pushunsigned(lua, u);
#endif
		}

		inline void push(lua_State* lua, lua_Number number)
		{
			lua_pushnumber(lua, number);
		}

		inline void push(lua_State* lua, const std::string& s)
		{
			lua_pushlstring(lua, s.c_str(), s.size());
		}

		inline void push(lua_State* lua, const char* s)
		{
			lua_pushstring(lua, s);
		}

		template <typename T>
		inline void set(lua_State* lua, T&& value, int index)
		{
			push(lua, std::forward<T>(value));
			lua_replace(lua, index);
		}

		inline void pushN(lua_State*)
		{
		}

		template <typename T, typename... Rest>
		inline void pushN(lua_State* lua, T value, Rest ... rest)
		{
			push(lua, std::forward<T>(value));
			pushN(lua, rest...);
		}

		inline void push(lua_State*, std::tuple<>)
		{
		}

		template <typename... T, std::size_t... N>
		inline void pushTupleImpl(lua_State* lua, const std::tuple<T...>& values, traits::Indices<N...>)
		{
			pushN(lua, std::get<N>(values)...);
		}

		template <typename... T>
		inline void push(lua_State* lua, const std::tuple<T...>& values)
		{
			pushTupleImpl(lua, values, typename traits::Indexer<sizeof...(T)>::Type());
		}

		template <typename R, typename... Args>
		inline void push(lua_State* lua, LuaFunction<R(Args ...)> func)
		{
			func.push(lua);
		}
	}

	class LuaRef
	{
	public:
		class Deleter
		{
		public:
			Deleter(lua_State* lua) : lua{ lua }
			{
			}

			void operator()(int* ref) const
			{
				luaL_unref(lua, LUA_REGISTRYINDEX, *ref);
				delete ref;
			}

		private:
			lua_State* lua;
		};

		LuaRef(lua_State* lua, int ref) : ref(new int{ ref }, Deleter{ lua })
		{
		}

		void push(lua_State* lua)
		{
			lua_rawgeti(lua, LUA_REGISTRYINDEX, *ref);
		}

	private:
		std::shared_ptr<int> ref;
	};

	struct FunctionBase
	{
		virtual ~FunctionBase() {}
		virtual int apply(lua_State *lua) = 0;

	protected:
		template <typename... Ts>
		inline std::tuple<Ts...> getArgs(lua_State *lua)
		{
			return getArgsImpl<Ts...>(lua, typename traits::Indexer<sizeof...(Ts)>::Type());
		}

		template <typename Result, typename... Args>
		inline Result invoke(std::function<Result(Args...)> func, std::tuple<Args...> args)
		{
			return invokeImpl(func, args, typename traits::Indexer<sizeof...(Args)>::Type());
		}

		static inline int pickAndApply(lua_State *lua)
		{
			auto func = static_cast<FunctionBase *>(lua_touserdata(lua, lua_upvalueindex(1)));
			return func->apply(lua);
		}

	private:
		template <typename... Ts, std::size_t... N>
		inline std::tuple<Ts...> getArgsImpl(lua_State *lua, traits::Indices<N...>)
		{
			return std::tuple<Ts...>{stack::getChecked(traits::Id<Ts>{}, lua, N + 1)...};
		}

		template <typename Result, typename... Args, std::size_t... N>
		inline Result invokeImpl(std::function<Result(Args...)> func, std::tuple<Args...> args, traits::Indices<N...>)
		{
			return func(std::get<N>(args)...);
		}
	};

	template <typename Result, typename... Args>
	class LuaFunction<Result(Args...)>
	{
	public:
		LuaFunction(int ref, lua_State* lua) : ref(lua, ref), lua(lua)
		{
		}

		Result operator()(Args... args)
		{
			ref.push(lua);
			stack::pushN(lua, args...);
			lua_call(lua, sizeof...(Args), 1);
			Result ret = stack::pop(traits::Id<Result>{}, lua);
			lua_settop(lua, 0);
			return ret;
		}

		void push(lua_State* lua)
		{
			ref.push(lua);
		}

	private:
		LuaRef ref;
		lua_State* lua;
	};

	template <typename... Args>
	class LuaFunction<void(Args...)>
	{
	public:
		LuaFunction(int ref, lua_State* lua) : ref(lua, ref), lua(lua)
		{
		}

		void operator()(Args ... args)
		{
			ref.push(lua);
			stack::pushN(lua, args...);
			lua_call(lua, sizeof...(Args), 1);
			lua_settop(lua, 0);
		}

		void push(lua_State* lua)
		{
			ref.push(lua);
		}

	private:
		LuaRef ref;
		lua_State* lua;
	};

	template <typename... Result, typename... Args>
	class LuaFunction<std::tuple<Result...>(Args ...)>
	{
	public:
		LuaFunction(int ref, lua_State* lua) : ref(lua, ref), lua(lua)
		{
		}

		std::tuple<Result...> operator()(Args ... args)
		{
			ref.push(lua);
			stack::pushN(lua, args...);
			lua_call(lua, sizeof...(Args), sizeof...(Result));
			return stack::popAndReset<Result...>(lua);
		}

		void push(lua_State* lua)
		{
			ref.push(lua);
		}

	private:
		LuaRef ref;
		lua_State* lua;
	};

	template <int N, typename Result, typename... Args>
	class StandaloneFunction : public FunctionBase
	{
		using Type = std::function<Result(Args ...)>;

	public:
		StandaloneFunction(lua_State*& lua, Result(*func)(Args ...)) : StandaloneFunction(lua, Type{ func })
		{
		}

		StandaloneFunction(lua_State*& lua, Type func) : func(func)
		{
			lua_pushlightuserdata(lua, static_cast<void *>(static_cast<FunctionBase *>(this)));
			lua_pushcclosure(lua, &pickAndApply, 1);
		}

		int apply(lua_State* lua) override
		{
			std::tuple<Args...> args = getArgs<Args...>(lua);
			Result value = invoke(func, args);
			stack::push(lua, std::forward<Result>(value));
			return N;
		}

	private:
		Type func;
	};

	template <typename... Args>
	class StandaloneFunction<0, void, Args...> : public FunctionBase
	{
		using Type = std::function<void(Args ...)>;

	public:
		StandaloneFunction(lua_State*& lua, void(*fun)(Args ...)):
			StandaloneFunction(lua, Type{ fun })
		{
		}

		StandaloneFunction(lua_State*& lua, Type func) : func(func)
		{
			lua_pushlightuserdata(lua, static_cast<void *>(static_cast<FunctionBase *>(this)));
			lua_pushcclosure(lua, &pickAndApply, 1);
		}

		int apply(lua_State* lua) override
		{
			std::tuple<Args...> args = getArgs<Args...>(lua);
			invoke(func, args);
			return 0;
		}

	private:
		Type func;
	};

	template <int N, typename T, typename Result, typename... Args>
	class ClassFunction : public FunctionBase
	{
		using Type = std::function<Result(T*, Args ...)>;

	public:
		ClassFunction(lua_State* lua, const std::string& name, const std::string& metatableName, Result(*func)(Args ...)) :
			ClassFunction(lua, name, metatableName, Type{ func })
		{
		}

		ClassFunction(lua_State* lua, const std::string& name, const std::string& metatableName, Type func) :
			func(func), name(name), metatableName(metatableName)
		{
			lua_pushlightuserdata(lua, static_cast<void *>(static_cast<FunctionBase *>(this)));
			lua_pushcclosure(lua, &pickAndApply, 1);
			lua_setfield(lua, -2, name.c_str());
		}

		int apply(lua_State* lua) override
		{
			std::tuple<T*> t = std::make_tuple(get(lua));
			std::tuple<Args...> args = getArgs<Args...>(lua);
			std::tuple<T*, Args...> pack = std::tuple_cat(t, args);
			Result value = invoke(func, pack);
			stack::push(lua, std::forward<Result>(value));
			return N;
		}

	private:
		Type func;
		std::string name;
		std::string metatableName;

		T* get(lua_State* lua)
		{
			T* ret = *static_cast<T **>(luaL_checkudata(lua, 1, metatableName.c_str()));
			lua_remove(lua, 1);
			return ret;
		}
	};

	template <typename T, typename... Args>
	class ClassFunction<0, T, void, Args...> : public FunctionBase
	{
		using Type = std::function<void(T*, Args ...)>;

	public:
		ClassFunction(lua_State* lua, const std::string& name, const std::string& metatableName, void(*func)(Args ...)) :
			ClassFunction(lua, name, metatableName, Type{ func })
		{
		}

		ClassFunction(lua_State* lua, const std::string& name, const std::string& metatableName, Type func) :
			func(func), name(name), metatableName(metatableName)
		{
			lua_pushlightuserdata(lua, static_cast<void *>(static_cast<FunctionBase *>(this)));
			lua_pushcclosure(lua, &pickAndApply, 1);
			lua_setfield(lua, -2, name.c_str());
		}

		int apply(lua_State* lua) override
		{
			std::tuple<T*> t = std::make_tuple(get(lua));
			std::tuple<Args...> args = getArgs<Args...>(lua);
			std::tuple<T*, Args...> pack = std::tuple_cat(t, args);
			invoke(func, pack);
			return 0;
		}

	private:
		Type func;
		std::string name;
		std::string metatableName;

		T* get(lua_State* lua)
		{
			T* ret = *static_cast<T **>(luaL_checkudata(lua, 1, metatableName.c_str()));
			lua_remove(lua, 1);
			return ret;
		}
	};

	template <typename T, typename... Args>
	class Constructor : public FunctionBase
	{
		using Type = std::function<void(lua_State*, Args ...)>;

	public:
		Constructor(lua_State* lua, const std::string& metatableName)
		{
			ctor = [metatableName](lua_State* l, Args ... args)
			{
				T** t = static_cast<T **>(lua_newuserdata(l, sizeof(T *)));
				*t = new T(args...);
				luaL_getmetatable(l, metatableName.c_str());
				lua_setmetatable(l, -2);
			};
			lua_pushlightuserdata(lua, static_cast<void *>(static_cast<FunctionBase *>(this)));
			lua_pushcclosure(lua, &pickAndApply, 1);
			lua_setfield(lua, -2, "new");
		}

		int apply(lua_State* lua) override
		{
			std::tuple<Args...> args = getArgs<Args...>(lua);
			auto pack = std::tuple_cat(std::make_tuple(lua), args);
			invoke(ctor, pack);
			return 1;
		}

	private:
		Type ctor;
	};

	template <typename T>
	class Destructor : public FunctionBase
	{
	public:
		Destructor(lua_State* lua, const std::string& metatableName) : metatableName(metatableName)
		{
			lua_pushlightuserdata(lua, static_cast<void *>(static_cast<FunctionBase *>(this)));
			lua_pushcclosure(lua, &pickAndApply, 1);
			lua_setfield(lua, -2, "__gc");
		}

		int apply(lua_State* lua) override
		{
			T* t = *static_cast<T **>(luaL_checkudata(lua, 1, metatableName.c_str()));
			delete t;
			return 0;
		}

	private:
		std::string metatableName;
	};

	template <int N, typename Result, typename... Args>
	class ObjectFunction : public FunctionBase
	{
		using Type = std::function<Result(Args...)>;

	public:
		ObjectFunction(lua_State *lua, const std::string &name, Result(*func)(Args...))
			: ObjectFunction(lua, name, Type{ func })
		{
		}

		ObjectFunction(lua_State *lua, const std::string &name, Type func) : func(func)
		{
			lua_pushlightuserdata(lua, reinterpret_cast<void *>(static_cast<FunctionBase *>(this)));
			lua_pushcclosure(lua, &pickAndApply, 1);
			lua_setfield(lua, -2, name.c_str());
		}

		int apply(lua_State *lua) override
		{
			std::tuple<Args...> args = getArgs<Args...>(lua);
			Result value = invoke(func, args);
			stack::push(lua, std::forward<Result>(value));
			return N;
		}

	private:
		Type func;
	};

	template <typename... Args>
	class ObjectFunction<0, void, Args...> : public FunctionBase
	{
		using Type = std::function<void(Args...)>;

	public:
		ObjectFunction(lua_State *lua, const std::string &name, void(*func)(Args...))
			: ObjectFunction(lua, name, Type{ func })
		{
		}

		ObjectFunction(lua_State *lua, const std::string &name, Type func) :
			func(func)
		{
			lua_pushlightuserdata(lua, reinterpret_cast<void *>(static_cast<FunctionBase *>(this)));
			lua_pushcclosure(lua, &pickAndApply, 1);
			lua_setfield(lua, -2, name.c_str());
		}

		int apply(lua_State *lua) override
		{
			std::tuple<Args...> args = getArgs<Args...>(lua);
			invoke(func, args);
			return 0;
		}

	private:
		Type func;
	};

	struct ObjectBase
	{
		virtual ~ObjectBase()
		{
		}
	};

	template <typename T, typename... Members>
	class Object : public ObjectBase
	{
	public:
		Object(lua_State* lua, T* t, Members ... members)
		{
			lua_createtable(lua, 0, sizeof...(Members));
			registerMembers(lua, t, members...);
		}

	private:
		std::vector<std::unique_ptr<FunctionBase>> funcs;

		template <typename M>
		void registerMember(lua_State* lua, T* t, const char* memberName, M T::* member)
		{
			registerMember(lua, t, memberName, member, typename std::is_const<M>::type{});
		}

		template <typename M>
		void registerMember(lua_State* lua, T* t, const char* memberName, M T::* member, std::false_type)
		{
			std::function<M()> get = [t, member]()
			{
				return t->*member;
			};
			funcs.emplace_back(new ObjectFunction<1, M>{ lua, std::string{ memberName }, get });

			std::function<void(M)> set = [t, member](M value)
			{
				t->*member = value;
			};
			funcs.emplace_back(new ObjectFunction<0, void, M>{ lua, std::string{ "set_" } + memberName, set });
		}

		template <typename M>
		void registerMember(lua_State* lua, T* t, const char* memberName, M T::* member, std::true_type)
		{
			std::function<M()> lambda_get = [t, member]()
			{
				return t->*member;
			};
			funcs.emplace_back(new ObjectFunction<1, M>{ lua, std::string{ memberName }, lambda_get });
		}

		template <typename Result, typename... Args>
		void registerMember(lua_State* lua, T* t, const char* funcName, Result(T::*func)(Args ...))
		{
			std::function<Result(Args ...)> lambda = [t, func](Args ... args)
			{
				return (t->*func)(args...);
			};
			funcs.emplace_back(new ObjectFunction<traits::Arity<Result>::value, Result, Args...>{ lua, std::string(funcName), lambda });
		}

		void registerMembers(lua_State*, T*)
		{
		}

		template <typename M, typename... Ms>
		void registerMembers(lua_State* lua, T* t, const char* name, M member, Ms ... members)
		{
			registerMember(lua, t, name, member);
			registerMembers(lua, t, members...);
		}
	};

	struct ClassBase
	{
		virtual ~ClassBase()
		{
		}
	};

	template <typename T, typename Constructor, typename... Members>
	class Class : public ClassBase
	{
		using Funcs = std::vector<std::unique_ptr<FunctionBase>>;

	public:
		Class(lua_State* lua, const std::string& name, Members ... members) : name(name)
		{
			luaL_newmetatable(lua, getMetatableName().c_str());
			registerDtor(lua);
			registerCtor(lua);
			registerMembers(lua, members...);
			lua_pushvalue(lua, -1);
			lua_setfield(lua, -1, "__index");
		}

	private:
		std::string name;
		std::unique_ptr<Constructor> ctor;
		std::unique_ptr<Destructor<T>> dtor;
		Funcs funcs;

		std::string getMetatableName()
		{
			return name + "_lib";
		}

		void registerCtor(lua_State* lua)
		{
			ctor.reset(new Constructor(lua, getMetatableName()));
		}

		void registerDtor(lua_State* lua)
		{
			dtor.reset(new Destructor<T>(lua, getMetatableName()));
		}

		template <typename M>
		void registerMember(lua_State* lua, const char* memberName, M T::* member)
		{
			registerMember(lua, memberName, member, typename std::is_const<M>::type{});
		}

		template <typename M>
		void registerMember(lua_State* lua, const char* memberName, M T::* member, std::false_type)
		{
			std::function<M(T*)> get = [member](T* t)
			{
				return t->*member;
			};
			funcs.emplace_back(new ClassFunction<1, T, M>{ lua, std::string{ memberName }, getMetatableName(), get });

			std::function<void(T*, M)> set = [member](T* t, M value)
			{
				(t->*member) = value;
			};
			funcs.emplace_back(
				new ClassFunction<0, T, void, M>{ lua, std::string("set_") + memberName, getMetatableName(), set }
			);
		}

		template <typename M>
		void registerMember(lua_State* lua, const char* memberName, M T::* member, std::true_type)
		{
			std::function<M(T*)> get = [member](T* t) { return t->*member; };
			funcs.emplace_back(
				new ClassFunction<1, T, M>{ lua, std::string{ memberName }, getMetatableName(), get }
			);
		}

		template <typename Result, typename... Args>
		void registerMember(lua_State* lua, const char* funcName, Result(T::*func)(Args ...))
		{
			std::function<Result(T*, Args ...)> lambda = [func](T* t, Args ... args)
			{
				return (t->*func)(args...);
			};
			funcs.emplace_back(
				new ClassFunction<traits::Arity<Result>::value, T, Result, Args...>
				{
					lua,
					std::string(funcName),
					getMetatableName(),
					lambda
				}
			);
		}

		template <typename Result, typename... Args>
		void registerMember(lua_State* lua, const char* funcName, Result(T::*func)(Args ...) const)
		{
			std::function<Result(const T*, Args ...)> lambda = [func](const T* t, Args ... args)
			{
				return (t->*func)(args...);
			};
			funcs.emplace_back(
				new ClassFunction<traits::Arity<Result>::value, const T, Result, Args...>
				{
					lua,
					std::string(funcName),
					getMetatableName(),
					lambda
				}
			);
		}

		void registerMembers(lua_State*)
		{
		}

		template <typename Member, typename... OtherMembers>
		void registerMembers(lua_State* lua, const char* name, Member member, OtherMembers ... members)
		{
			registerMember(lua, name, member);
			registerMembers(lua, members...);
		}
	};

	class Registry
	{
	public:
		Registry(lua_State *lua) : lua(lua)
		{
		}

		template <typename L>
		void reg(L lambda)
		{
			reg(typename LambdaTraits<L>::Function(lambda));
		}

		template <typename TResult, typename... TArgs>
		void reg(std::function<TResult(TArgs...)> func)
		{
			auto tmp = std::unique_ptr<FunctionBase>(new StandaloneFunction<traits::Arity<TResult>::value, TResult, TArgs...>{ lua, func });
			funcs.push_back(std::move(tmp));
		}

		template <typename TResult, typename... TArgs>
		void reg(TResult(*func)(TArgs...))
		{
			auto tmp = std::unique_ptr<FunctionBase>(new StandaloneFunction<traits::Arity<TResult>::value, TResult, TArgs...>{ lua, func });
			funcs.push_back(std::move(tmp));
		}

		template <typename T, typename... TFunctions>
		void reg(T &t, std::tuple<TFunctions...> funcs)
		{
			reg(t, funcs, typename traits::Indexer<sizeof...(TFunctions)>::Type{});
		}

		template <typename T, typename... TFunctions, size_t... N>
		void reg(T &t, std::tuple<TFunctions...> funcs, traits::Indices<N...>)
		{
			regObject(t, std::get<N>(funcs)...);
		}

		template <typename T, typename... TFunctions>
		void regObject(T &t, TFunctions... funcs)
		{
			auto tmp = std::unique_ptr<ObjectBase>(new Object<T, TFunctions...>{ lua, &t, funcs... });
			objects.push_back(std::move(tmp));
		}

		template <typename T, typename... TCtorArgs, typename... TFunctions, size_t... N>
		void regClass(const std::string &name, std::tuple<TFunctions...> funcs, traits::Indices<N...>)
		{
			regClassWorker<T, TCtorArgs...>(name, std::get<N>(funcs)...);
		}

		template <typename T, typename... TCtorArgs, typename... TFunctions>
		void regClassWorker(const std::string &name, TFunctions... funcs)
		{
			auto tmp = std::unique_ptr<ClassBase>(new Class<T, Constructor<T, TCtorArgs...>, TFunctions...>{ lua, name, funcs... });
			classes.push_back(std::move(tmp));
		}

	private:
		template <typename T>
		struct LambdaTraits : LambdaTraits<decltype(&T::operator())>
		{
		};

		template <typename THost, typename TResult, typename... TArgs>
		struct LambdaTraits<TResult(THost::*)(TArgs...) const>
		{
			using Function = std::function<TResult(TArgs...)>;
		};

		std::vector<std::unique_ptr<FunctionBase>> funcs;
		std::vector<std::unique_ptr<ObjectBase>> objects;
		std::vector<std::unique_ptr<ClassBase>> classes;
		lua_State *lua;
	};

	class Script;

	class Accessor
	{
		using VoidFunc = std::function<void()>;
		using FuncOfInt = std::function<void(int)>;
		using FuncOfFunc = std::function<void(VoidFunc)>;

	public:
		Accessor(const Accessor& other) :
			lua(other.lua),
			registry(other.registry),
			currentName(other.currentName),
			traversal(other.traversal),
			get(other.get),
			put(other.put)
		{
		}

		~Accessor()
		{
			if (functor != nullptr)
			{
				traverse();
				get();
				(*functor)(0);
			}
			lua_settop(lua, 0);
		}

		bool operator==(Accessor& other) = delete;

		template <typename... TArgs>
		const Accessor& operator()(TArgs ... args) const
		{
			auto argsTuple = std::make_tuple(std::forward<TArgs>(args)...);
			auto tmp = new FuncOfInt([this, argsTuple](int retCount)
			{
				stack::push(lua, argsTuple);
				lua_call(lua, sizeof...(TArgs), retCount);
			});
			functor.reset(std::move(tmp));
			return *this;
		}

		template <typename L>
		void operator=(L lambda) const
		{
			traverse();
			auto push = [this, lambda]()
			{
				registry.reg(lambda);
			};
			put(push);
			lua_settop(lua, 0);
		}

		void operator=(bool b) const
		{
			traverse();
			auto push = [this, b]()
			{
				stack::push(lua, b);
			};
			put(push);
			lua_settop(lua, 0);
		}

		void operator=(int i) const
		{
			traverse();
			auto push = [this, i]()
			{
				stack::push(lua, i);
			};
			put(push);
			lua_settop(lua, 0);
		}

		void operator=(unsigned i) const
		{
			traverse();
			auto push = [this, i]()
			{
				stack::push(lua, i);
			};
			put(push);
			lua_settop(lua, 0);
		}

		void operator=(lua_Number n) const
		{
			traverse();
			auto push = [this, n]()
			{
				stack::push(lua, n);
			};
			put(push);
			lua_settop(lua, 0);
		}

		void operator=(const std::string& s) const
		{
			traverse();
			auto push = [this, s]()
			{
				stack::push(lua, s);
			};
			put(push);
			lua_settop(lua, 0);
		}

		template <typename TResult, typename... TArgs>
		void operator=(std::function<TResult(TArgs ...)> func)
		{
			traverse();
			auto push = [this, func]()
			{
				registry.reg(func);
			};
			put(push);
		}

		template <typename TResult, typename... TArgs>
		void operator=(TResult(*func)(TArgs ...))
		{
			traverse();
			auto push = [this, func]()
			{
				registry.reg(func);
			};
			put(push);
		}

		void operator=(const char* s) const
		{
			traverse();
			auto push = [this, s]()
			{
				stack::push(lua, std::string{ s });
			};
			put(push);
			lua_settop(lua, 0);
		}

		template <typename T, typename... Functions>
		void setObject(T& t, Functions ... functions)
		{
			traverse();
			auto fns = std::make_tuple(functions...);
			auto push = [this, &t, &fns]()
			{
				registry.reg(t, fns);
			};
			put(push);
			lua_settop(lua, 0);
		}

		template <typename T, typename... CtorArgs, typename... Functions>
		void setClass(Functions ... functions)
		{
			traverse();
			auto funcsTuple = std::make_tuple(functions...);
			auto push = [this, &funcsTuple]()
			{
				typename traits::Indexer<sizeof...(Functions)>::Type d;
				registry.regClass<T, CtorArgs...>(currentName, funcsTuple, d);
			};
			put(push);
			lua_settop(lua, 0);
		}

		template <typename... Result>
		std::tuple<Result...> getTuple() const
		{
			traverse();
			get();
			(*functor)(sizeof...(Result));
			return stack::popAndReset<Result...>(lua);
		}

		operator bool() const
		{
			return castSimple<bool>();
		}

		operator int() const
		{
			return castSimple<int>();
		}

		operator unsigned() const
		{
			return castSimple<unsigned>();
		}

		operator lua_Number() const
		{
			return castSimple<lua_Number>();
		}

		operator std::string() const
		{
			return castSimple<std::string>();
		}

		template <typename R, typename... Args>
		operator LuaFunction<R(Args ...)>()
		{
			return castSimple<LuaFunction<R(Args...)>>();
		}

		Accessor operator[](const char* name) const
		{
			auto newName = currentName + "." + name;
			ensureTable();
			auto newTraversal = traversal;
			newTraversal.push_back(get);
			VoidFunc get = [this, name]()
			{
				lua_getfield(lua, -1, name);
			};
			FuncOfFunc put = [this, name](VoidFunc func)
			{
				func();
				lua_setfield(lua, -2, name);
				lua_pop(lua, 1);
			};
			return Accessor{ lua, registry, newName, newTraversal, get, put };
		}

		Accessor operator[](int index) const
		{
			auto name = currentName + "." + std::to_string(index);
			ensureTable();
			auto newTraversal = traversal;
			newTraversal.push_back(get);
			VoidFunc get = [this, index]()
			{
				lua_pushinteger(lua, index);
				lua_gettable(lua, -2);
			};
			FuncOfFunc put = [this, index](VoidFunc func)
			{
				lua_pushinteger(lua, index);
				func();
				lua_settable(lua, -3);
				lua_pop(lua, 1);
			};
			return Accessor{ lua, registry, name, newTraversal, get, put };
		}

	private:
		friend class Script;

		lua_State* lua;
		Registry& registry;
		std::string currentName;
		std::vector<VoidFunc> traversal;

		VoidFunc get;
		FuncOfFunc put;

		mutable std::unique_ptr<FuncOfInt> functor;

		Accessor(lua_State* s, Registry& r, const std::string& name, std::vector<VoidFunc> traversal, VoidFunc get, FuncOfFunc put) :
			lua(s), registry(r), currentName(name), traversal(traversal), get(get), put(put), functor{ nullptr }
		{
		}

		Accessor(lua_State* s, Registry& r, const char* name):
			lua(s), registry(r), currentName(name), functor{ nullptr }
		{
			get = [this, name]() { lua_getglobal(lua, name); };
			put = [this, name](VoidFunc func)
			{
				func();
				lua_setglobal(lua, name);
			};
		}

		template <typename T>
		inline T castSimple() const
		{
			traverse();
			get();
			if (functor != nullptr)
			{
				(*functor)(1);
				functor.reset();
			}
			auto ret = stack::pop(traits::Id<T>{}, lua);
			lua_settop(lua, 0);
			return ret;
		}

		void ensureTable() const
		{
			traverse();
			get();
			if (lua_istable(lua, -1) == 0)
			{
				lua_pop(lua, 1);
				auto lambda = [this]() { lua_newtable(lua); };
				put(lambda);
			}
			else
				lua_pop(lua, 1);
		}

		void traverse() const
		{
			for (auto& func : traversal)
				func();
		}
	};

	inline bool operator==(const Accessor& s, const char* c)
	{
		return std::string{ c } == std::string(s);
	}

	inline bool operator==(const char* c, const Accessor& s)
	{
		return std::string{ c } == std::string(s);
	}

	template <typename T>
	inline bool operator==(const Accessor& s, T&& t)
	{
		return T(s) == t;
	}

	template <typename T>
	inline bool operator==(T&& t, const Accessor& s)
	{
		return T(s) == t;
	}

	template <typename... T>
	class Tuple
	{
	public:
		Tuple(T&... args) : tuple(args...)
		{
		}

		void operator=(const Accessor &s)
		{
			tuple = s.getTuple<typename std::remove_reference<T>::type...>();
		}

	private:
		std::tuple<T&...> tuple;
	};

	template <typename... T>
	Tuple<T&...> tie(T&... args)
	{
		return Tuple<T&...>(args...);
	}

	class Script
	{
	public:
		Script() : Script(false)
		{
		}

		Script(bool openLibs) : lua(nullptr)
		{
			lua = luaL_newstate();
			if (lua == nullptr)
				throw 0;
			if (openLibs)
				luaL_openlibs(lua);
			registry.reset(new Registry(lua));
		}

		Script(const Script &other) = delete;
		Script &operator=(const Script &other) = delete;

		Script(Script &&other) :
			lua(other.lua),
			registry(std::move(other.registry))
		{
			other.lua = nullptr;
		}

		~Script()
		{
			if (lua != nullptr)
				lua_close(lua);
			lua = nullptr;
		}

		bool runFile(const std::string &file)
		{
			return !luaL_dofile(lua, file.c_str());
		}

		void openLib(const std::string& modname, lua_CFunction openf)
		{
			luaL_requiref(lua, modname.c_str(), openf, 1);
		}

		void push()
		{
		}

		template <typename T, typename... Ts>
		void push(T &&value, Ts&&... values)
		{
			stack::push(lua, std::forward<T>(value));
			push(std::forward<Ts>(values)...);
		}

		template <typename T>
		T get(int index) const
		{
			return stack::get(traits::Id<T>{}, lua, index);
		}

		bool isNull(const std::string &global)
		{
			lua_getglobal(lua, global.c_str());
			const auto result = lua_isnil(lua, -1);
			lua_pop(lua, 1);
			return result;
		}

		Accessor operator[](const char *name)
		{
			return Accessor(lua, *registry, name);
		}

		void operator()(const char *code)
		{
			luaL_dostring(lua, code);
			lua_settop(lua, 0);
		}

		void gc()
		{
			lua_gc(lua, LUA_GCCOLLECT, 0);
		}

		int getStackSize() const
		{
			return lua_gettop(lua);
		}

	private:
		lua_State *lua;
		std::unique_ptr<Registry> registry;
	};
}