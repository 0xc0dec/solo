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
#ifndef __SLB3_VALUE_PRIMITIVES__
#define __SLB3_VALUE_PRIMITIVES__

namespace SLB3 {

namespace internal {
  // This function handles integers, and also som valid conversions
  // to integers (like enums)
  lua_Integer GetInteger(lua_State *L, int index);
}

  //--- Specializations Functions -----------------------------------------
  template<>
  struct Value<lua_CFunction> {
    typedef lua_CFunction GetType;
    static void Push(lua_State *L,lua_CFunction func) {
      lua_pushcclosure(L, func, 0);
    }
    static lua_CFunction Get(lua_State *L, int index) {
      return lua_tocfunction(L, index);
    }
  };

  //--- Specializations ---------------------------------------------------
  template<>
  struct Value<void> {
    typedef void GetType;
    static void Push(...) {}
    static void Get(...) {}
  };

  // Value specialization for <char>
  template<>
  struct Value<char> {
    typedef char GetType;
    static void Push(lua_State *L, const char v) {
      lua_pushinteger(L,v);
    }
    static char Get(lua_State *L, int p) {
      char v = (char) internal::GetInteger(L,p);
      return v;
    }
  };

  template<> struct Value<char&> : public Value<char> {};
  template<> struct Value<const char&> : public Value<char> {};
  template<> struct Value<const char>  : public Value<char> {};

  // Value specialization for <unsigned char>
  template<>
  struct Value<unsigned char> {
    typedef unsigned char GetType;
    static void Push(lua_State *L, const unsigned char v) {
      lua_pushinteger(L,v);
    }
    static unsigned char Get(lua_State *L, int p) {
      unsigned char v = (unsigned char) internal::GetInteger(L,p);
      return v;
    }
  };
  template<> struct Value<unsigned char&> : public Value<unsigned char> {};
  template<> struct Value<const unsigned char&> : public Value<unsigned char> {};
  template<> struct Value<const unsigned char>  : public Value<unsigned char> {};

  // Value specialization for <short>
  template<>
  struct Value<short>
  {
    typedef short GetType;
    static void Push(lua_State *L, const short v) {
      lua_pushinteger(L,v);
    }
    static short Get(lua_State *L, int p) {
      short v = (short) internal::GetInteger(L,p);
      return v;
    }
  };

  template<> struct Value<short&> : public Value<short> {};
  template<> struct Value<const short&> : public Value<short> {};
  template<> struct Value<const short>  : public Value<short> {};

  // Value specialization for <unsigned short>
  template<>
  struct Value<unsigned short> {
    typedef unsigned short GetType;

    static void Push(lua_State *L, const unsigned short v) {
      lua_pushinteger(L,v);
    }
    static unsigned short Get(lua_State *L, int p) {
      unsigned short v = (unsigned short) internal::GetInteger(L,p);
      return v;
    }
  };

  template<> struct Value<unsigned short&> : public Value<unsigned short> {};
  template<> struct Value<const unsigned short&> : public Value<unsigned short> {};
  template<> struct Value<const unsigned short>  : public Value<unsigned short> {};

  // Value specialization for <int>
  template<>
  struct Value<int> {
    typedef int GetType;
    static void Push(lua_State *L, const int v) {
      lua_pushinteger(L,v);
    }
    static int Get(lua_State *L, int p) {
      int v = (int) internal::GetInteger(L,p);
      return v;
    }
  };

  template<> struct Value<int&> : public Value<int> {};
  template<> struct Value<const int&> : public Value<int> {};
  template<> struct Value<const int>  : public Value<int> {};

  // Value specialization for <unsigned int>
  template<>
  struct Value<unsigned int> {
    typedef unsigned int GetType;
    static void Push(lua_State *L, const unsigned int v) {
      lua_pushinteger(L,v);
    }
    static unsigned int Get(lua_State *L, int p) {
      unsigned int v = static_cast<unsigned int>(internal::GetInteger(L,p));
      return v;
    }
  };

  template<> struct Value<unsigned int&> : public Value<unsigned int> {};
  template<> struct Value<const unsigned int&> : public Value<unsigned int> {};
  template<> struct Value<const unsigned int>  : public Value<unsigned int> {};
  

  template<>
  struct Value<long> {
    typedef long GetType;
    static void Push(lua_State *L, const long v) {
      lua_pushinteger(L,v);
    }
    static long Get(lua_State *L, int p) {
      long v = (long) internal::GetInteger(L,p);
      return v;
    }
  };

  template<> struct Value<long&> : public Value<long> {};
  template<> struct Value<const long&> : public Value<long> {};
  template<> struct Value<const long>  : public Value<long> {};
  

  /* unsigned long == unsigned int */
  template<>
  struct Value<unsigned long>
  {
    typedef unsigned long GetType;
    static void Push(lua_State *L, const unsigned long v) {
      lua_pushnumber(L,v);
    }

    static unsigned long Get(lua_State *L, int p) {
      unsigned long v = (unsigned long) lua_tonumber(L,p);
      return v;
    }
  };

  template<> struct Value<unsigned long&> : public Value<unsigned long> {};
  template<> struct Value<const unsigned long&> : public Value<unsigned long> {};
  template<> struct Value<const unsigned long>  : public Value<unsigned long> {};
  
/*
  template<>
  struct Value<unsigned long long>
  {
    typedef unsigned long long GetType;
    static void Push(lua_State *L, unsigned long long v) {
      lua_pushnumber(L,(lua_Number) v);
    }

    static unsigned long long Get(lua_State *L, int p) {
      unsigned long long v = (unsigned long long) lua_tonumber(L,p);
      return v;
    }
  };

  template<> struct Value<unsigned long long&> : public Value<unsigned long long> {};
  template<> struct Value<const unsigned long long&> : public Value<unsigned long long> {};
  */

  // Value specialization for <double>
  template<>
  struct Value<double> {
    typedef double GetType;
    static void Push(lua_State *L, const double v) {
      lua_pushnumber(L,v);
    }
    static double Get(lua_State *L, int p) {
      double v = (double) lua_tonumber(L,p);
      return v;
    }
  };

  template<> struct Value<double&> : public Value<double> {};
  template<> struct Value<const double&> : public Value<double> {};
  template<> struct Value<const double>  : public Value<double> {};
  
  // Value specialization for <float>
  template<>
  struct Value<float> {
    typedef float GetType;
    static void Push(lua_State *L, const float v) {
      lua_pushnumber(L,v);
    }
    static float Get(lua_State *L, int p) {
      float v = (float) lua_tonumber(L,p);
      return v;
    }
  };

  template<> struct Value<float&> : public Value<float> {};
  template<> struct Value<const float&> : public Value<float> {};
  template<> struct Value<const float> : public Value<float> {};
  
  
  // Value specialization for <bool>
  template<>
  struct Value<bool> {
    typedef bool GetType;
    static void Push(lua_State *L, const bool v) {
      lua_pushboolean(L,v);
    }
    static bool Get(lua_State *L, int p) {
      bool v = (lua_toboolean(L,p) != 0);
      return v;
    }
  };


  template<> struct Value<bool&> : public Value<bool> {};
  template<> struct Value<const bool&> : public Value<bool> {};
  template<> struct Value<const bool>  : public Value<bool> {};

  /*
  template<>
  struct Value<std::string>
  {
    typedef std::string GetType;
    static void Push(lua_State *L, const std::string &v)
    {
      lua_pushlstring(L, v.data(), v.size());
    }

    static std::string Get(lua_State *L, int p)
    {
      size_t len;
      const char* v = (const char*) lua_tolstring(L,p, &len);
      return std::string(v, len);
    }
  };

  template<> struct Value<std::string&> : public Value<std::string> {};
  template<> struct Value<const std::string&> : public Value<std::string> {};
*/

  // Value specialization for <const char*>
  template<>
  struct Value<const char*> {
    typedef const char* GetType;
    static void Push(lua_State *L, const char* v)
    {
      if (v) {
        lua_pushstring(L,v);
      } else {
        lua_pushnil(L);
      }
    }
    static const char* Get(lua_State *L, int p) {
      const char* v = (const char*) lua_tostring(L,p);
      return v;
    }
  };

  template<>
  struct Value<const unsigned char*> {
    typedef const unsigned char* GetType;
    static void Push(lua_State *L, const unsigned char* v) {
      if (v) {
        lua_pushstring(L,(const char*)v);
      } else {
        lua_pushnil(L);
      }
    }
    static const unsigned char* Get(lua_State *L, int p)
    {
      const unsigned char* v = (const unsigned char*) lua_tostring(L,p);
      return v;
    }
  };

}

#endif
