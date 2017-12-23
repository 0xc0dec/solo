/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include <LuaIntf.h>
#include <map>
#include <vector>

namespace LuaIntf
{
    LUA_USING_SHARED_PTR_TYPE(std::shared_ptr)
    LUA_USING_LIST_TYPE(std::vector)
    LUA_USING_MAP_TYPE(std::map) // TODO unordered_map
}

using namespace LuaIntf;

#define BEGIN_CLASS(module, klass) module.beginClass<klass>(#klass)
#define BEGIN_CLASS_RENAMED(module, klass, name) module.beginClass<klass>(name)
#define BEGIN_CLASS_EXTEND(module, klass, base) module.beginExtendClass<klass, base>(#klass)

#define REG_CTOR(binding, ...) binding.addConstructor(LUA_ARGS(__VA_ARGS__))

#define REG_FIELD(binding, klass, name) binding.addVariable(#name, &klass::name, true)

#define REG_META_METHOD(binding, name, func) binding.addMetaFunction(name, func)
#define REG_METHOD(binding, klass, name) binding.addFunction(#name, &klass::name)
#define REG_METHOD_OVERLOADED(binding, klass, name, nameStr, resultType, modifier, ...) \
        binding.addFunction(nameStr, static_cast<resultType(klass::*)(__VA_ARGS__)modifier>(&klass::name))

#define REG_STATIC_METHOD(binding, klass, name) binding.addStaticFunction(#name, &klass::name)
#define REG_STATIC_METHOD_OVERLOADED(binding, klass, name, nameStr, resultType, modifier, ...) \
        binding.addStaticFunction(nameStr, static_cast<resultType(*)(__VA_ARGS__)modifier>(&klass::name))

#define REG_FREE_FUNC_AS_METHOD(binding, func) binding.addFunction(#func, func)
#define REG_FREE_FUNC_AS_METHOD_RENAMED(binding, func, name) binding.addFunction(name, func)
#define REG_FREE_FUNC_AS_STATIC_FUNC_RENAMED(binding, func, name) binding.addStaticFunction(name, func)

// TODO refactor these
#define REG_METHOD_NULLABLE_1ST_ARG(binding, klass, method, argType) \
    binding.addFunction(#method, \
        [](klass *obj, LuaRef arg) \
        { \
            auto unwrapped = arg.isValid() && arg.toPtr() \
                ? arg.toValue<argType>() \
                : nullptr; \
            obj->method(unwrapped); \
        })

#define REG_METHOD_NULLABLE_2ND_ARG(binding, klass, method, firstArgType, secondArgType) \
    binding.addFunction(#method, \
        [](klass *obj, firstArgType arg1, LuaRef arg2) \
        { \
            auto unwrapped = arg2.isValid() && arg2.toPtr() \
                ? arg2.toValue<secondArgType>() \
                : nullptr; \
            obj->method(arg1, unwrapped); \
        })

#define REG_MODULE_CONSTANT(module, holder, constant) module.addConstant(#constant, holder::constant)
