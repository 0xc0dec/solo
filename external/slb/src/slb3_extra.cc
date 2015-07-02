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

#include <SLB3/declaration.h>
#include <SLB3/extra/errorhandler.h>
#include <SLB3/extra/script.h>
#include <SLB3/extra/coroutinemanager.h>
#include <SLB3/extra/package.h>

#include <iostream>
#include <cstdio>

namespace SLB3 {
namespace extra {

//------------------------------------------------------------------------------
// Script Class 
//------------------------------------------------------------------------------

static void *ScriptDefaultAlloc(void *ud, void *ptr, size_t os, size_t ns);
static void DefaultPrintCallback(Script *s, const char *str, size_t length);

Script::Script()
    : lua_state_(NULL)
    , allocator_(NULL)
    , allocator_ud_(NULL)
    , load_default_libs_(true)
    , print_callback_(DefaultPrintCallback)
    , event_callback_(NULL) {}

Script::~Script() {
  finish();
}

void Script::init() {
  initWithAllocator(ScriptDefaultAlloc, NULL);
}

void Script::initWithAllocator(lua_Alloc func, void *ud) {
  if (!lua_state_) {
    allocator_ = func;
    allocator_ud_ = ud;
    doInitialization();
  }
}

void Script::finish() {
  if (lua_state_) {
    if (event_callback_) event_callback_(this, kEvent_Finish, NULL);
    lua_close(lua_state_);
    lua_state_ = NULL;
  }
}

void Script::callGC() {
  if (lua_state_) {
    lua_gc(lua_state_, LUA_GCCOLLECT, 0);
  }
}
  
size_t Script::memUsage() {
  size_t result  = 0;
  if (lua_state_) {
    int r = lua_gc(lua_state_, LUA_GCCOUNT, 0);
    result = r;
  }
  return result;
}

bool Script::doFile(const char *filename, ErrorHandler *error_handler) {
  int top = lua_gettop(lua_state_);
  return doExecution(top, luaL_loadfile(lua_state_, filename), error_handler);
}

bool Script::doString(const char *code, const char *hint,
    ErrorHandler *error_handler) {

  int top = lua_gettop(lua_state_);

  if (hint) {
    lua_pushstring(lua_state_, "--");
    lua_pushstring(lua_state_, hint);
    lua_pushstring(lua_state_, "\n");
    lua_pushstring(lua_state_, code);
    lua_concat(lua_state_, 4);
  } else {
    lua_pushstring(lua_state_,code);
  }

  return doExecution(top
      , luaL_loadstring(lua_state_, lua_tostring(lua_state_,-1))
      , error_handler);
}

void Script::doInitialization() {
  assert(lua_state_ == NULL && "Invalid call to doInitialization");
  lua_state_ = lua_newstate(allocator_, allocator_ud_);
  if (load_default_libs_) luaL_openlibs(lua_state_);
  // replace print method
  lua_pushlightuserdata(lua_state_, this);
  lua_pushcclosure(lua_state_,PrintHook,1);
  lua_setglobal(lua_state_, "print");
  // notify new luaVM
  if (event_callback_) event_callback_(this, kEvent_Init, NULL);
}

bool Script::doExecution(int top, int load_result, ErrorHandler *err_handler) {
  DefaultErrorHandler default_error_handler;
  if (err_handler == NULL) err_handler = &default_error_handler;

  lua_State *L = lua_state_;
  bool result = true;

  switch(load_result) {
    case LUA_OK:
      /* no errors */
      break;
    case LUA_ERRFILE:
      err_handler->internal_launchError(ErrorHandler::kFileNotFound_Error,
          lua_tostring(L,-1));
      result = false;
      break;
    case LUA_ERRSYNTAX:
      err_handler->internal_launchError(ErrorHandler::kSyntax_Error,
          lua_tostring(L,-1));
      result = false;
      break;
    case LUA_ERRMEM: 
      err_handler->internal_launchError(ErrorHandler::kOutOfMemory_Error,
          "Out Of Memory");
      result = false;
      break;
    default:
      assert(!"Invalid case option");
  }

  if(result && err_handler->call(L, 0, 0)) {
    if (event_callback_) event_callback_(this, kEvent_Error, err_handler);
    result = false;
  }

  lua_settop(L,top);
  return result;
}

int Script::PrintHook(lua_State *L) {
  Script *script = reinterpret_cast<Script*>(lua_touserdata(L,lua_upvalueindex(1)));
  int n = lua_gettop(L);  /* number of arguments */
  int i;
  lua_getglobal(L, "tostring");
  for (i=1; i<=n; i++) {
    const char *s;
    size_t l;
    lua_pushvalue(L, -1);  /* function to be called */
    lua_pushvalue(L, i);   /* value to print */
    lua_call(L, 1, 1);
    s = lua_tolstring(L, -1, &l);  /* get result */
    if (s == NULL)
      return luaL_error(L,
         LUA_QL("tostring") " must return a string to " LUA_QL("print"));
    if (i>1) script->print_callback_(script,"\t", 1);
    script->print_callback_(script,s, l);
    lua_pop(L, 1);  /* pop result */
  }
  script->print_callback_(script,"\n",1);
  return 0;
}

void *ScriptDefaultAlloc(void *ud, void *ptr, size_t os, size_t ns) {
  (void) ud; // not used
  (void) os; // not used
  if (0 == ns) {
    free(ptr);
    return NULL;
  } else {
    return realloc(ptr, ns);
  }
}

void DefaultPrintCallback(Script *s, const char *str, size_t length) {
  (void) s; // not used
  std::cout.write(str,length);
}

//------------------------------------------------------------------------------
// Abstract Error Handler
//------------------------------------------------------------------------------

int ErrorHandler::call(lua_State *L, int nargs, int nresults) {
  // insert the error handler before the arguments
  int base = lua_gettop(L) - nargs;
  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, InternalStackHandler, 1);
  lua_insert(L, base);

  // call the function
  int result = lua_pcall(L, nargs, nresults, base);

  // remove the error handler
  lua_remove(L, base);

  // return the previous result
  return result;
}

void ErrorHandler::internal_launchError(const ErrorType type, const char *msg) {
  begin(type, msg);
  end();
}

const char *ErrorHandler::SE_name() const        { return debug_.name; }
const char *ErrorHandler::SE_nameWhat() const    { return debug_.namewhat; }
const char *ErrorHandler::SE_what() const        { return debug_.what; }
const char *ErrorHandler::SE_source() const      { return debug_.source; }
const char *ErrorHandler::SE_shortSource() const { return debug_.short_src; }
int ErrorHandler::SE_currentLine() const      { return debug_.currentline; }
int ErrorHandler::SE_numberOfUpvalues() const { return debug_.nups; } 
int ErrorHandler::SE_lineDefined() const      { return debug_.linedefined; } 
int ErrorHandler::SE_lastLineDefined() const  { return debug_.lastlinedefined; } 
bool ErrorHandler::SE_isTailCall() const      { return (debug_.istailcall != 0); } 

int ErrorHandler::InternalStackHandler(lua_State *L) {
  ErrorHandler *EH = reinterpret_cast<ErrorHandler*>(
      lua_touserdata(L,lua_upvalueindex(1)));
  EH->internal_processLuaStackAfterError(L);
  const char *error = EH->last_error();
  lua_pushstring(L, error);
  return 1;
}

void ErrorHandler::internal_processLuaStackAfterError(lua_State *L) {
  lua_state_ = L;
  const char *error = lua_tostring(lua_state_, -1);
  begin(kRuntime_Error, error);
  for ( int level = 0; lua_getstack(lua_state_, level, &debug_ ); level++) {
    if (lua_getinfo(L, "Slnu", &debug_) ) {
      stackElement(level);  
    } else {
      assert(!"[ERROR using Lua DEBUG INTERFACE]");
    }
  }
  end();
  lua_state_ = NULL;
}

//------------------------------------------------------------------------------
// Buffer Error Handler
//------------------------------------------------------------------------------

BufferErrorHandler::BufferErrorHandler(char *buffer, size_t buffer_size)
    : buffer_(buffer)
    , buffer_size_(buffer_size)
    , buffer_pos_(0) {}

void BufferErrorHandler::begin(const ErrorType err_type, const char *error) {
  buffer_pos_ = 0;
  buffer_[0] = '\0';

  append("-- Lua Error: ");
  switch(err_type) {
    case kRuntime_Error:
      append("SLB3 Exception ");
      break;
    case kSyntax_Error:
      append("SLB3-Error: Syntax Error");
      break;
    case kOutOfMemory_Error:
      append("SLB3-Error: Out of memory");
      break;
    case kFileNotFound_Error:
      append("SLB3-Error: File Not found");
      break;
  }

  append(
  "\n-------------------------------------------------------------- Error --\n");
  append(error);
  append(
  "\n---------------------------------------------------------- Traceback --\n");
}

void BufferErrorHandler::end() {
  append(
  "-----------------------------------------------------------------------\n");
}

void BufferErrorHandler::stackElement(int level) {
  append("\t [ ").append(level).append(" (").append(SE_what()).append(") ] ");
  if (SE_isTailCall()) {
    append("tail call");
  } else {
    append(SE_shortSource());
    int currentline = SE_currentLine();
    if (currentline >= 0 ) append(":").append(currentline); 

    const char *name = SE_name();
    if (name) {
      append(" @ ").append(name);
    } else {
      append(" @ (unknown func_name)");
    }
    if (SE_nameWhat()) append("(").append(SE_nameWhat()).append(")");
  }
  append("\n");
}

BufferErrorHandler& BufferErrorHandler::append(const char *txt) {
  size_t len = strlen(txt);
  if (len + buffer_pos_ < buffer_size_) {
    strncat(buffer_+buffer_pos_, txt, len);
    buffer_pos_ += len;
  }
  return *this;
}

BufferErrorHandler& BufferErrorHandler::append(int value) {
#if _MSC_VER
#define snprintf _snprintf
#endif
  static const unsigned num_chars_for_int_base_10 = 1+sizeof(int)*31/100;
  char tmp[num_chars_for_int_base_10];
  snprintf(tmp, num_chars_for_int_base_10, "%d", value);
  append(tmp);
  return *this;
#if _MSC_VER
#undef snprintf
#endif
}

//------------------------------------------------------------------------------
// Coroutine Manager
//------------------------------------------------------------------------------

enum CoroutineState {
  kCoroutineState_Invalid,
  kCoroutineState_Begin,
  kCoroutineState_ToBeResumed,
  kCoroutineState_Yielded
};

struct CoroutineManager::Data {
  unsigned total_coroutines;
  unsigned active_coroutines;
  int active_coroutines_table_ref;
  int suspended_coroutines_table_ref;
  int coroutines_table_ref;
};

CoroutineManager::CoroutineManager()
    : lua_state_(NULL)
    , data_(NULL)
    , event_mask_(0) {
  update_tmp_.table = -1;
}


CoroutineManager::~CoroutineManager() { finish(); }

void CoroutineManager::init(lua_State *s) {
  if (lua_state_) finish();
  lua_state_ = s;

  // Setup Internal Data
  data_ = (Data*) lua_newuserdata(lua_state_, sizeof(Data));
  // create a metatable to identify __GC on the data (which means the lua_state
  // is being closed before finish() call)
  lua_newtable(lua_state_);
  lua_pushlightuserdata(lua_state_, this);
  lua_pushcclosure(lua_state_, lua_detect_GC, 1);
  lua_setfield(lua_state_, -2, "__gc");
  lua_setmetatable(lua_state_, -2);
  // Place Internal Data into the registry
  lua_rawsetp(lua_state_, LUA_REGISTRYINDEX, this);
  data_->active_coroutines = 0;
  data_->total_coroutines = 0;
  // Create Active coroutines table
  lua_newtable(lua_state_);
  data_->active_coroutines_table_ref = luaL_ref(lua_state_, LUA_REGISTRYINDEX);
  // Suspended coroutines table
  lua_newtable(lua_state_);
  data_->suspended_coroutines_table_ref = luaL_ref(lua_state_, LUA_REGISTRYINDEX);
  // Coroutines table
  lua_newtable(lua_state_);
  data_->coroutines_table_ref = luaL_ref(lua_state_, LUA_REGISTRYINDEX);

  // call actual implementation
  init_implementation();
}

void CoroutineManager::pushLua_waitFor(lua_State *L, int id) {
  // Push waitFor function
  lua_pushlightuserdata(L, this);
  lua_pushinteger(L, id);
  lua_pushcclosure(L, lua_waitfor_call, 2);
}

void CoroutineManager::pushLua_async(lua_State *L) {
  // Push async function
  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, lua_async_call, 1);
}

void CoroutineManager::finish() {
  if (lua_state_) {
    lua_State *L = lua_state_;
    int top = lua_gettop(L);
    finish_implementation();
    // -- Notify abort of pending coroutines -----------------------------
    if (event_mask_ & kEvent_CoroutineAbort) {
      notifyAbortAllCoroutines(data_->active_coroutines_table_ref);
      notifyAbortAllCoroutines(data_->suspended_coroutines_table_ref);
    }
    // -- Remove coroutine tables ----------------------------------------
    luaL_unref(L, LUA_REGISTRYINDEX, data_->active_coroutines_table_ref);
    luaL_unref(L, LUA_REGISTRYINDEX, data_->suspended_coroutines_table_ref);
    luaL_unref(L, LUA_REGISTRYINDEX, data_->coroutines_table_ref);
    // -- mark the current lua_state, and data invalid, and remove DATA --
    lua_state_ = NULL;
    data_ = NULL;
    lua_pushnil(L);
    lua_rawsetp(L, LUA_REGISTRYINDEX, this);
    // -- End ------------------------------------------------------------
    lua_settop(L, top);
  }
}

void CoroutineManager::update() {
  lua_State *L = lua_state_;
  if (!L) return; 

  // -- Prepare update ----------------------------------------------------
  int top = lua_gettop(L);
  lua_rawgeti(L, LUA_REGISTRYINDEX, data_->active_coroutines_table_ref);
  update_tmp_.table = luaL_ref(L, LUA_REGISTRYINDEX);
  update_tmp_.last_thread = NULL;
  update_tmp_.last_thread_status = kCoroutineState_Invalid;
  // Once we have the active coroutine table, replace the with an empty one
  lua_newtable(L);
  lua_rawseti(L, LUA_REGISTRYINDEX, data_->active_coroutines_table_ref);
  assert(lua_gettop(L) == top && "Invalid lua stack balance before update");
  // -- Call update -------------------------------------------------------
  update_implementation();
  // -- Post update -------------------------------------------------------
  top = lua_gettop(L);
  lua_rawgeti(L, LUA_REGISTRYINDEX, data_->active_coroutines_table_ref);
  while (update_pullNextActiveCoroutine()) {
    lua_pushlightuserdata(L, update_tmp_.last_thread);
    lua_pushinteger(L, update_tmp_.last_thread_status);
    lua_rawset(L, -3);
  }
  // put all remaining elements back to the 
  luaL_unref(L, LUA_REGISTRYINDEX, update_tmp_.table);
  lua_pop(L,1); // remove active coroutine's table
  assert(lua_gettop(L) == top && "Invalid lua stack balance after update");
  // -- Reset values ------------------------------------------------------
  update_tmp_.table = -1;
  update_tmp_.last_thread = NULL;
  update_tmp_.last_thread_status = kCoroutineState_Invalid;
}

CoroutineManager::Thread CoroutineManager::update_pullNextActiveCoroutine() {
  lua_State *L = lua_state_;
  int top = lua_gettop(L);
  Thread result = NULL;

  lua_rawgeti(L, LUA_REGISTRYINDEX, update_tmp_.table);
  // Push last key, or nil to start from the beginning
  if (update_tmp_.last_thread) {
    lua_pushlightuserdata(L, update_tmp_.last_thread);
  } else {
    lua_pushnil(L);
  }
  if (lua_next(L,-2)) {
    result = (Thread) lua_topointer(L,-2);
    update_tmp_.last_thread = result;
    update_tmp_.last_thread_status = (int) lua_tointeger(L, -1);
    lua_pop(L,3);
  } else {
    lua_pop(L,1);
  }
  assert(lua_gettop(L) == top && "Invalid update_pullNext... invalid stack");
  return result;
}

void CoroutineManager::update_resumeCoroutine() {
  Thread thread = update_tmp_.last_thread;
  int state = update_tmp_.last_thread_status;

  // Check with the callback if we process the thread
  int result = -99999;
  switch(state) {
    case kCoroutineState_Begin:
      result = lua_resume(thread, NULL, lua_gettop(thread)-1);
      break;
    case kCoroutineState_ToBeResumed:
      result = lua_resume(thread, NULL, lua_gettop(thread));
      break;
    default:
      assert(!"Invalid state for active coroutine");
  }
  // Now process the result: it can yield, finish or give an error
  switch(result) {
    case LUA_YIELD:
      // if the coroutine yielded, that must be as a result of
      // waitFor. waitFor already takes care to place functions on the
      // table where they belong. We don't need to do anything here.
      return;
    case LUA_OK:
      dispatchEvent(kEvent_CoroutineFinishes, thread, NULL);
      break;
    default:
      // error, use a Default error handler to process the coroutine stack
      DefaultErrorHandler err;
      err.internal_processLuaStackAfterError(thread);
      lua_settop(thread, 0);
      dispatchEvent(kEvent_CoroutineError, thread, err.last_error());
      break;
  }
  // Coroutine has finished (either ok, or with errors) so remove it from
  // the system.
  data_->active_coroutines--;
  data_->total_coroutines--;
  // remove it from the coroutines table
  lua_rawgeti(lua_state_, LUA_REGISTRYINDEX, data_->coroutines_table_ref);
  lua_pushnil(lua_state_);
  lua_rawsetp(lua_state_, -2, thread);
  lua_pop(lua_state_, 1);
}

bool CoroutineManager::signalCoroutine(Thread thread) {
  bool result = false;
  if (lua_state_) {
    int top = lua_gettop(lua_state_);
    int suspended_t = top+1;
    int active_t = top+2;
    lua_rawgeti(lua_state_, LUA_REGISTRYINDEX,
        data_->suspended_coroutines_table_ref);
    lua_rawgeti(lua_state_, LUA_REGISTRYINDEX,
        data_->active_coroutines_table_ref);

    lua_rawgetp(lua_state_, suspended_t, thread);
    if (!lua_isnil(lua_state_,-1)) {
      lua_pushnil(lua_state_);
      lua_rawsetp(lua_state_, suspended_t, thread);
      lua_pushnumber(lua_state_, kCoroutineState_ToBeResumed);
      lua_rawsetp(lua_state_, active_t, thread);
      // everything ok
      dispatchEvent(kEvent_CoroutineSignaled, thread, NULL);
      result = true;
      data_->active_coroutines++;
    }

    lua_settop(lua_state_, top);
  }
  return result;
}

bool CoroutineManager::abortCoroutine(Thread thread) {
  bool result = false;
  if (lua_state_) {
    int top = lua_gettop(lua_state_);
    int tables[] = {
      data_->suspended_coroutines_table_ref,
      data_->active_coroutines_table_ref };

    for(unsigned i = 0; !result && (i < 2); ++i) {
      lua_rawgeti(lua_state_, LUA_REGISTRYINDEX, tables[i]);
      lua_rawgetp(lua_state_, -1, thread);
      if (!lua_isnil(lua_state_, -1)) {
        // element found, abort the coroutine
        dispatchEvent(kEvent_CoroutineAbort, thread, NULL);
        data_->total_coroutines--;
        if (i == 1) data_->active_coroutines--;
        // remove the element from the table
        lua_pushnil(lua_state_);
        lua_rawsetp(lua_state_, -3, thread);
        // Now remove the thread from the coroutines table
        lua_rawgeti(lua_state_, LUA_REGISTRYINDEX, data_->coroutines_table_ref);
        lua_pushnil(lua_state_);
        lua_rawsetp(lua_state_, -2, thread);
        lua_pop(lua_state_,1); // and pop the coroutines table
        // search over
        result = true;
      }
    }
    lua_settop(lua_state_, top);
  }
  return result;
}

void CoroutineManager::set_event_mask(unsigned mask) {
  event_mask_ = mask;
}

unsigned CoroutineManager::total_coroutines() const {
  if (data_) {
    return data_->total_coroutines;
  }
  return 0;
}

unsigned CoroutineManager::active_coroutines() const {
  if (data_) {
    return data_->active_coroutines;
  }
  return 0;
}

void CoroutineManager::dispatchEvent(
    Event type,
    Thread id,
    const char *error) {
  if (type & event_mask_) {
    event_implementation(type, id, error);
  }
}

void CoroutineManager::notifyAbortAllCoroutines(int table_ref) {
  lua_State *L = lua_state();
  lua_rawgeti(L, LUA_REGISTRYINDEX, table_ref);
  lua_pushnil(L);
  while(lua_next(L,-2)) {
    event_implementation(kEvent_CoroutineAbort,
        (Thread)lua_topointer(L,-2), NULL);
    lua_pop(L,1);
  }
  lua_pop(L,1);
}

CoroutineManager::Thread CoroutineManager::createCoroutine(
    lua_State *L, int index) {
  Thread thread = NULL;
  index = lua_absindex(L, index);
  if (lua_isfunction(L, index)) {
    // create the coroutine and store it
    lua_rawgeti(L, LUA_REGISTRYINDEX, data_->coroutines_table_ref);
    thread = lua_newthread(L);
    lua_rawsetp(L, -2, thread);
    lua_pop(L,1); // remove the coroutines table

    // keep the coroutine as active (using its pointer as key)
    lua_rawgeti(L, LUA_REGISTRYINDEX, data_->active_coroutines_table_ref);
    lua_pushnumber(L, kCoroutineState_Begin);
    lua_rawsetp(L, -2, thread);
    lua_pop(L, 1); // remove active coroutines table

    // move a copy of the function to the new created thread
    lua_pushvalue(L, index);
    lua_xmove(L, thread, 1);

    // update count
    data_->active_coroutines++;
    data_->total_coroutines++;
    dispatchEvent(kEvent_CoroutineCreated, thread, NULL);
  }
  return thread;
}

int CoroutineManager::lua_async_call(lua_State *L) {
  CoroutineManager *cm =
      (CoroutineManager*) lua_touserdata(L, lua_upvalueindex(1));
  Thread thread = cm->createCoroutine(L,1);
  if (thread) {
    // remove the first function
    lua_remove(L,1);
    // move all arguments to the new created thread
    lua_xmove(L, thread, lua_gettop(L));
  } else {
  return luaL_error(L,
      "Invalid call to async, arg 1 can't be used as coroutine");
  }
  return 0;
}

int CoroutineManager::lua_waitfor_call(lua_State *L) {
  CoroutineManager *cm =
      (CoroutineManager*) lua_touserdata(L, lua_upvalueindex(1));
  int waitfor_id = lua_tointeger(L, lua_upvalueindex(2));

  if (lua_pushthread(L)) {
    return luaL_error(L,
        "Can't call waitfor-like function from the main thread...");
  }
  Thread thread = lua_tothread(L,-1);
  lua_pop(L,1);

  WaitForResult wres = cm->waitFor_implementation(thread, waitfor_id);
  switch(wres) {
    case kW_ContinueExecution:
      return lua_gettop(L);
    case kW_Error:
      return lua_error(L);
    case kW_WaitForSignal:
      lua_rawgeti(L, LUA_REGISTRYINDEX, cm->data_->suspended_coroutines_table_ref);
      lua_pushnumber(L, kCoroutineState_Yielded);
      lua_rawsetp(L,-2, thread);
      lua_pop(L,1); // remove suspended coroutine's table
      cm->dispatchEvent(kEvent_CoroutineSuspended, thread, NULL);
      cm->data_->active_coroutines--;
      return lua_yield(L, lua_gettop(L));
    case kW_WaitUntilNextUpdate:
      lua_rawgeti(L, LUA_REGISTRYINDEX, cm->data_->active_coroutines_table_ref);
      lua_pushnumber(L, kCoroutineState_ToBeResumed);
      lua_rawsetp(L,-2, thread);
      lua_pop(L,1); // remove active coroutine's table
      // all returned values kept on the stack
      return lua_yield(L, lua_gettop(L));
    case kW_AbortCoroutine:
      cm->dispatchEvent(kEvent_CoroutineAbort, thread, lua_tostring(L,-1));
      cm->data_->active_coroutines--;
      cm->data_->total_coroutines--;
      return lua_yield(L, 0);
  }
  luaL_error(L, "Invalid result from waitfor-like callback... ");
  return 0;
}

int CoroutineManager::lua_detect_GC(lua_State *L) {
  CoroutineManager *cm =
      (CoroutineManager*) lua_touserdata(L, lua_upvalueindex(1));
  if (cm->lua_state_ != NULL) {
    cm->dispatchEvent(kEvent_PrematureCloseOfLuaState, NULL, NULL);
    cm->lua_state_ = NULL;
    cm->data_ = NULL;
  }
  return 0;
}

//------------------------------------------------------------------------------
// Package: Lua Functions
//------------------------------------------------------------------------------

void PreloadSLB3Package(lua_State *L, const char *name) {
  luaL_Reg functions[] = {
    { "getinfo", getinfo },
    { "classes", classes },
    { NULL, NULL}
  };
  lua_getglobal(L, "package");
  lua_getfield(L, -1, "loaded");
  luaL_newlibtable(L, functions);
  luaL_setfuncs(L, functions, 0);
  lua_setfield(L, -2, name);
  lua_pop(L,2);
}

int GetInfoReturnedValue(lua_State *L);
int GetInfoArgs(lua_State *L);

void PushInfo(lua_State *L, const BaseInfo *info) {
  if (info) {
    lua_newtable(L);
    int t = lua_gettop(L);
    switch(info->type()) {
      case BaseInfo::kType_unknown:
        lua_pushstring(L, "unknown");
        break;
      case BaseInfo::kType_void:
        lua_pushstring(L, "void");
        break;
      case BaseInfo::kType_primitive_value:
        lua_pushstring(L, "primitive_value");
        break;
      case BaseInfo::kType_class:
        lua_pushstring(L, "class");
        break;
      case BaseInfo::kType_function:
        lua_pushstring(L, "function");
        break;
    }
    lua_setfield(L, t, "type");
    lua_pushstring(L, info->name());
    lua_setfield(L, t, "name");
    lua_pushinteger(L, info->num_args());
    lua_setfield(L, t, "num_args");

    lua_pushlightuserdata(L, (void*) info->type_returned());
    lua_pushcclosure(L, GetInfoReturnedValue, 1);
    lua_setfield(L, t, "type_returned");
    
    lua_pushlightuserdata(L, (void*) info->args());
    lua_pushinteger(L, info->num_args());
    lua_pushcclosure(L, GetInfoArgs, 2);
    lua_setfield(L, t, "args");
  } else {
    lua_pushnil(L);
  }
}

int GetInfoReturnedValue(lua_State *L) {
  const BaseInfo *info = 
      reinterpret_cast<const BaseInfo*>(lua_touserdata(L, lua_upvalueindex(1)));
  PushInfo(L, info);
  return 1;
}

int GetInfoArgs(lua_State *L) {
  const BaseInfo **info = 
      reinterpret_cast<const BaseInfo**>(lua_touserdata(L, lua_upvalueindex(1)));
  const int max_index = lua_tointeger(L, lua_upvalueindex(2));
  const int index = lua_tointeger(L,1);
  if (index < max_index && index >= 0) {
    PushInfo(L, info[index]);
    return 1;
  }
  return 0;
}

int getinfo(lua_State *L) {
  const BaseInfo *info = GetInfo(L,1);
  PushInfo(L, info);
  return 1;
}

int classes(lua_State *L) {
  lua_getfield(L, LUA_REGISTRYINDEX, "SLB3_Classes");
  return 1;
}

} /* extra */
} /* SLB */
